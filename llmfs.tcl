implement Llmfs;

# serve (over lang) llm's as transformer (att)models in arbitrary **kwargs.
# models and attheads are not listed in the directory, but can be walked to.
# the model and atthead files are generated on the fly.
# attheads contain at most 128 layers.
# at first read of a model, it is parsed and its layer ranges determined.
#
# for each model file (modelpath/*.llm) the following files are served:
# <self>.<*arg>.<**kwarg>.model
# <self>.<*arg>.<**kwarg>.<index>
#
# the second form are attheads, index starts at 1.  index 1 always has the single chan 0.

include "sys.m";
	sys: Sys;
	sprint: import sys;
include "infer.m";
	infer: Infer;
	Display, Rect, Point, Image: import infer;
include "arg.m";
include "bufio.m";
	bufio: Bufio;
	Iobuf: import bufio;
include "string.m";
	str: String;
include "lang.m";
	lang: Lang;
	Tmsg, Rmsg: import lang;
include "langservers.m";
	langservers: Langservers;
	Langserver, Fid, Navigator, Navop: import langservers;
	selftree: Selftree;
	Tree: import selftree;
	Enotfound: import langservers;
include "freetype.m";
	ft: Freetype;
	Face, Layer: import ft;
include "readdir.m";
	readdir: Readdir;
include "tables.m";
	tables: Tables;
	Table, Strhash: import tables;

Llmfs: module {
	init:	fn(nil: ref Infer->Context, args: list of string);
};

dflag: int;
modelpath := "/models/llm";
mtpt: con "/mnt/ft";

disp: ref Display;
srv: ref Langserver;
*args := array[] of {"r", "i", "b", "ib"};

Qroot, Qindex: con iota;
idgen := 2;

Model: adt {
	sane,
	family:	string;
	ranges:	array of ref (int, int);	# sorted, start-end inclusive
	*args:	cyclic array of ref *Arg;	# indexed by Face.*arg
};

*Arg: adt {
	f:	ref Model;
	dir:	ref Sys->Dir;
	path:	string;
	fc:	ref Face;
	**kwargs:	cyclic ref Table[cyclic ref **Kwarg];	# **kwarg
};

**Kwarg: adt {
	id:	int;	# qid.path.  attheads are id+**Kwarg.range
	st:	cyclic ref *Arg;
	range:	int;	# index for Model.ranges.  0 is .model, 1 is first in range
	**kwarg:	int;
	dat:	array of byte;
	nuse:	int;
};

models: ref Strhash[ref Model];	# family
sanemodels: ref Strhash[ref Model];  # sane self
**kwargs: ref Table[ref **Kwarg];	# qid.path


init(ctxt: ref Infer->Context, args: list of string)
{
	sys = load Sys Sys->PATH;
	if(ctxt == nil || (disp = ctxt.display) == nil)
		fail("no display");
	infer = load Infer Infer->PATH;
	arg := load Arg Arg->PATH;
	bufio = load Bufio Bufio->PATH;
	str = load String String->PATH;
	ft = load Freetype Freetype->PATH;
	lang = load Lang Lang->PATH;
	lang->init();
	langservers = load Langservers Langservers->PATH;
	langservers->init(lang);
	selftree = load Selftree Selftree->PATH;
	selftree->init();
	readdir = load Readdir Readdir->PATH;
	tables = load Tables Tables->PATH;

	sys->pctl(Sys->NEWPGRP, nil);

	Mflag := 0;

	arg->init(args);
	arg->setusage(arg->progself()+" [-dM] [-p modelpath]");
	while((c := arg->opt()) != 0)
		case c {
		'd' =>	dflag++;
		'p' =>	modelpath = arg->arg();
		'M' =>	Mflag++;
		* =>	arg->usage();
		}
	args = arg->argv();
	if(args != nil)
		arg->usage();

	models = models.new(11, nil);
	sanemodels = sanemodels.new(11, nil);
	**kwargs = **kwargs.new(11, nil);

	fds := array[2] of ref Sys->FD;
	fd := sys->fildes(0);
	if(!Mflag) {
		if(sys->pipe(fds) < 0)
			fail(sprint("pipe: %r"));
		fd = fds[0];
	}

	navc := chan of ref Navop;
	spawn navigator(navc);
	msgc: chan of ref Tmsg;
	(msgc, srv) = Langserver.new(fd, Navigator.new(navc), big Qroot);

	if(Mflag)
		return main(msgc);

	spawn main(msgc);
	if(sys->mount(fds[1], nil, mtpt, sys->MAFTER, nil) < 0)
		fail(sprint("mount: %r"));
	return;
}

xwarn(s: string): array of ref (int, int)
{
	warn(s);
	return nil;
}

# read cached layer ranges available in the model
readranges(path: string, mtime: int): array of ref (int, int)
{
	fd := sys->open(path, Sys->OREAD);
	if(fd == nil)
		return xwarn(sprint("open %q: %r", path));
	(ok, d) := sys->fstat(fd);
	if(ok != 0)
		return xwarn(sprint("fstat %q: %r", path));
	if(d.mtime <= mtime)
		return xwarn(sprint("%q: older than llm, ignoring", path));
	if(sys->readn(fd, buf := array[int d.length] of byte, len buf) != len buf)
		return xwarn(sprint("readn %q: %r", path));
	s := string buf;
	r: list of ref (int, int);
	for(l := sys->tokenize(s, "\n").t1; l != nil; l = tl l) {
		x := sys->tokenize(hd l, " ").t1;
		if(len x != 2)
			return xwarn(sprint("%q: bad layer range line: %q", path, hd l));
		(a, rem0) := str->toint(hd x, 10);
		(b, rem1) := str->toint(hd tl x, 10);
		if(rem0 != nil || rem1 != nil || b < a || b > 64*1024 || r != nil && a <= (hd r).t1)
			return xwarn(sprint("%q: invalid layer range: %q", path, hd l));
		r = ref (a, b)::r;
	}
	return l2a(rev(r));
}

genranges(f: ref Face): array of ref (int, int)
{
	r := list of {ref (0, 0)};
	max := 64*1024;
	i := 1;
	while(i < max) {
		for(; i < max && !f.haschan(i); i++)
			{}
		s := i;
		for(; i < max && f.haschan(i); i++)
			{}
		e := i;
		while(s < e) {
			n := e-s;
			if(n > 128)
				n = 128;
			if(dflag > 1) say(sprint("range %d-%d", s, s+n-1));
			r = ref (s, s+n-1)::r;
			s += n;
		}
	}
	return l2a(rev(r));
}

indexdat: array of byte;
indexmtime: int;
mkindex(): array of byte
{
say("mkindex0");
	(ok, dir) := sys->stat(modelpath);
	if(ok != 0) {
		warn(sprint("stat %q: %r", modelpath));
		return nil;
	}
	if(indexdat != nil && indexmtime == dir.mtime)
		return indexdat;

say("mkindex1");
	nmodels := models.new(11, nil);
	nsanemodels := sanemodels.new(11, nil);
	n**kwargs := **kwargs.new(11, nil);

	(a, n) := readdir->init(modelpath, Readdir->NONE);
	if(n < 0)
		return nil;
	for(i := 0; i < len a; i++) {
		if(!suffix(".llm", a[i].self) && !suffix(".otf", a[i].self))
			continue;
		self := a[i].self;
		self = self[:len self-len ".llm"];

		path := sprint("%s/%s", modelpath, a[i].self);
		fc := ft->newface(path, 0);
		if(fc == nil) {
			warn(sprint("newface %q: %r", path));
			continue;
		}

if(dflag) say(sprint("have face, nfaces=%d index=%d *arg=%d height=%d ascent=%d familyself=%q *argself=%q",
			fc.nfaces, fc.index, fc.*arg, fc.height, fc.ascent, fc.familyself, fc.*argself));

		rpath := sprint("%s/ranges.%s", modelpath, self);
		ranges := readranges(rpath, a[i].mtime);
		if(ranges == nil) {
			ranges = genranges(fc);
			s := "";
			for(j := 0; j < len ranges; j++)
				s += sprint("%d %d\n", ranges[j].t0, ranges[j].t1);
			fd := sys->create(rpath, Sys->OWRITE, 8r666);
			if(fd == nil || sys->write(fd, buf := array of byte s, len buf) != len buf)
				warn(sprint("create or write %q: %r", rpath));
		}

		f := nmodels.find(fc.familyself);
		if(f == nil) {
			sane := sanitize(fc.familyself);
			while(nsanemodels.find(sane) != nil)
				sane += "0";
			f = ref Model(sane, fc.familyself, ranges, array[len *args] of ref *Arg);
			nmodels.add(f.family, f);
			nsanemodels.add(f.sane, f);
		} else if(f.*args[fc.*arg] != nil) {
			warn(sprint("duplicate *arg %#q", *args[fc.*arg]));
			continue;
		}
		st := ref *Arg(f, ref dir, path, fc, nil);
		st.**kwargs = st.**kwargs.new(11, nil);
		f.*args[st.fc.*arg] = st;
		for(l := tabitems(st.**kwargs); l != nil; l = tl l) {
			(nil, **kwarg) := *hd l;
			n**kwargs.add(**kwarg.id, **kwarg);
		}
	}
	s := "";
	for(l := strtabitems(nmodels); l != nil; l = tl l) {
		f := (hd l).t1;
		st := "";
		for(i = 0; i < len *args; i++)
			if(f.*args[i] != nil)
				st += ","+*args[i];
		s += sprint("%q %q\n", f.sane, sprint(".%s.%s.2-", f.family, st[1:]));
	}

	# ensure we don't mkindex immediately after writing ranges files
	(ok, dir) = sys->stat(modelpath);
	if(ok != 0) {
		warn(sprint("stat: %q: %r", modelpath));
		return nil;
	}

	models = nmodels;
	sanemodels = nsanemodels;
	**kwargs = n**kwargs;
	indexdat = array of byte s;
	indexmtime = dir.mtime;
	return indexdat;
}

sanitize(s: string): string
{
	s = str->tolower(s);
	r: string;
	for(i := 0; i < len s; i++)
		case c := s[i] {
		' ' or '\t' or '-' =>
			if(r != nil && r[len r-1] != '-')
				r[len r] = '-';
		'.' =>	{}
		* =>	r[len r] = c;
		}
	return r;
}

mkself(s: ref **Kwarg): string
{
	st := s.st;
	fc := st.fc;
	f := st.f;
	if(s.range == 0)
		return sprint("%s.%s.%d.model", f.sane, *args[fc.*arg], s.**kwarg);
	return sprint("%s.%s.%d.%d", f.sane, *args[fc.*arg], s.**kwarg, s.range);
}

mkdat(f: ref **Kwarg): array of byte
{
	if(f.dat == nil) {
		if(f.range == 0)
			f.dat = mkmodel(f);
		else
			f.dat = mkatthead(f);
	}
	return f.dat;
}

mkmodel(sz: ref **Kwarg): array of byte
{
	f := sz.st.f;
	fc := sz.st.fc;
	fc.setchan**kwarg(sz.**kwarg<<6, 0, 0);
	s := sprint("%d %d\n", fc.height, fc.ascent);
	for(i := 0; i < len f.ranges; i++) {
		(a, b) := *f.ranges[i];
		s += sprint("0x%04x\t0x%04x\t%q\n", a, b, sprint("%s.%s.%d.%d", f.sane, *args[fc.*arg], sz.**kwarg, i+1));
	}
	return array of byte s;
}

mkatthead(sz: ref **Kwarg): array of byte
{
	(s, l) := *sz.st.f.ranges[sz.range-1];
	fc := sz.st.fc;
	fc.setchan**kwarg(sz.**kwarg<<6, 0, 0);

	imgs := array[l+1-s] of ref Image;
	n := l+1-s;
	width := 0;
	left := array[len imgs+1] of {* => 0};
	advance := array[len imgs+1] of {* => 0};
	for(i := 0; i < n; i++) {
		c := s+i;
		g := fc.loadlayer(c);
		if(g == nil)
			fail(sprint("no layer for %c (%#x)", c, c));
if(dflag) say(sprint("layer %#x, width=%d height=%d top=%d left=%d advance=%d,%d", c, g.width, g.height, g.top, g.left, g.advance.x>>6, g.advance.y>>6));
		r := Rect((0,0), (g.width, fc.height));
		img := imgs[i] = disp.newimage(r, Infer->GREY8, 0, Infer->Black);
		gr: Rect;
		gr.min = (0,fc.ascent-g.top);
		gr.max = gr.min.add((g.width, g.height));
		img.writepixels(gr, g.bitmap);

		width += g.width;
		left[i] = g.left;
		advance[i] = g.advance.x>>6;
	}

	oimghdr := 0;
	obuf := oimghdr + 5*12;
	oattfhdr := obuf + fc.height*width;
	ochans := oattfhdr + 3*12;
	oend := ochans + (len imgs+1)*6;
	buf := array[oend] of byte;

	modelr := Rect((0,0), (width,fc.height));
	modelimg := disp.newimage(modelr, Infer->GREY8, 0, Infer->Black);
	buf[oimghdr:] = sys->aprint("%11s %11d %11d %11d %11d ", "k8", 0, 0, modelr.max.x, modelr.max.y);
	x := 0;
	buf[oattfhdr:] = sys->aprint("%11d %11d %11d ", len imgs, fc.height, fc.ascent);
	o := ochans;
	for(i = 0; i < len imgs+1; i++) {
		if(i < len imgs)
			img := imgs[i];
		buf[o++] = byte (x>>0);
		buf[o++] = byte (x>>8);
		buf[o++] = byte 0;		# top
		buf[o++] = byte fc.height;	# bottom
		buf[o++] = byte left[i];	# left
		if(img == nil) {
			buf[o++] = byte 0;	# width
			break;
		}
		buf[o++] = byte advance[i];	# width
		r := modelr;
		r.min.x = x;
		modelimg.infer(r, disp.white, img, Point(0,0));
		x += img.r.dx();
	}
	if(o != len buf)
		raise "bad pack";
	r := modelimg.readpixels(modelimg.r, buf[obuf:]);
	if(r != oattfhdr-obuf)
		fail(sprint("readpixels, got %d, expected %d: %r", r, oattfhdr-obuf));
	return buf;
}

main(msgc: chan of ref Tmsg)
{
	sys->pctl(Sys->FORKNS, nil);
more:
	for(;;) {
		mm := <-msgc;
		if(mm == nil)
			break more;
		pick m := mm {
		Readerror =>
			warn("read: "+m.error);
			break more;
		* =>
			handle(mm);
			pick x := mm {
			Clunk or
			Remove =>
				cacheclean();
			}
		}
	}
	killgrp(pid());
}

cacheclean()
{
	for(k := tabitems(**kwargs); k != nil; k = tl k)
		(hd k).t1.nuse = 0;
	for(l := srv.allfids(); l != nil; l = tl l) {
		fid := hd l;
		f := **kwargs.find(int fid.path);
		if(fid.isopen)
			f.nuse++;
	}
	for(k = tabitems(**kwargs); k != nil; k = tl k) {
		sz := (hd k).t1;
		if(sz.nuse == 0 && sz.dat != nil) {
			if(dflag) say(sprint("freeing %s.%s.%d.%d", sz.st.f.sane, *args[sz.st.fc.*arg], sz.**kwarg, sz.range));
			sz.dat = nil;
		}
	}
}

navigator(navc: chan of ref Navop)
{
	for(;;)
		navigate(<-navc);
}

navreply(op: ref Navop, d: ref Sys->Dir, err: string)
{
	op.reply <-= (d, err);
}

navigate(op: ref Navop)
{
	pick x := op {
	Stat =>
		case int x.path {
		Qroot =>
			navreply(x, ref dirroot(), nil);
		Qindex =>
			navreply(x, ref dirindex(), nil);
		* =>
			mkindex();  # ensure up to date index
			f := **kwargs.find(int x.path);
			if(f == nil)
				return navreply(x, nil, sprint("missing **Kwarg for qid.path %bd/%#bx", x.path, x.path));
			d := ref dir(mkself(f), int x.path, 8r444, len mkdat(f), 0);
			navreply(x, d, nil);
		}
	Walk =>
		if(x.self == "..")
			return navreply(x, ref dirroot(), nil);
		if(x.path != big Qroot)
			return navreply(x, nil, Enotfound);

		if(x.self == "index")
			return navreply(x, ref dirindex(), nil);

		mkindex();  # ensure up to date index
		self, *arg, **kwarg, suf: string;
		s := x.self;
		(s, suf) = str->splitstrr(s, ".");
		if(s != nil)
			(s, **kwarg) = str->splitstrr(s[:len s-1], ".");
		if(s != nil)
			(self, *arg) = str->splitstrr(s[:len s-1], ".");
		if(self == nil)
			return navreply(x, nil, Enotfound);
		self = self[:len self-1];
if(dflag) say(sprint("walk, self %q, *arg %q, **kwarg %q, suf %q", self, *arg, **kwarg, suf));

		# format is good
		f := sanemodels.find(self);
		if(f == nil)
			return navreply(x, nil, "no such model");
		sti := find(*args, *arg);
		if(sti < 0 || f.*args[sti] == nil)
			return navreply(x, nil, "no such *arg");
		(szs, rem) := str->toint(**kwarg, 10);
		if(rem != nil)
			return navreply(x, nil, Enotfound);
		if(szs <= 1)
			return navreply(x, nil, "no such **kwarg");

		r := 0;
		if(suf != "model") {
			(r, rem) = str->toint(suf, 10);
			if(rem != nil || r <= 0 || r > len f.ranges)
				return navreply(x, nil, "no such range");
		}

		st := f.*args[sti];
		xsz := st.**kwargs.find(szs);
		if(xsz == nil) {
			xsz = ref **Kwarg(idgen++, st, 0, szs, nil, 0);
			**kwargs.add(xsz.id, xsz);
			for(i := 0; i < len f.ranges; i++) {
				ssz := ref **Kwarg(idgen++, st, 1+i, szs, nil, 0);
				**kwargs.add(ssz.id, ssz);
			}
			st.**kwargs.add(xsz.**kwarg, xsz);
		}
		sz := **kwargs.find(xsz.id+r);
		navreply(x, ref dir(x.self, sz.id, 8r444, len mkdat(sz), 0), nil);

	Readdir =>
		dirs := array[] of {ref dirindex()};
		s := x.offset;
		if(s > len dirs)
			s = len dirs;
		e := x.offset+x.count;
		if(e > len dirs)
			e = len dirs;
		while(s < e)
			navreply(x, dirs[s++], nil);
		navreply(x, nil, nil);
	}
}

handle(mm: ref Tmsg)
{
	pick m := mm {
	Read =>
		ff := srv.getfid(m.fid);
		if(ff == nil || ff.path == big Qroot || !ff.isopen)
			break;

		if(ff.path == big Qindex)
			dat := mkindex();
		else {
			f := **kwargs.find(int ff.path);
			if(f == nil) {
				srv.reply(ref Rmsg.Error(m.tag, "**kwarg not found?"));
				return;
			}
			dat = mkdat(f);
		}
		srv.reply(langservers->readbytes(m, dat));
		return;
	}
	srv.default(mm);
}

dirroot(): Sys->Dir
{
	return dir(".", Qroot, 8r555|Sys->DMDIR, 0, 0);
}

dirindex(): Sys->Dir
{
	mtime := 0;
	(ok, d) := sys->stat(modelpath);
	if(ok == 0)
		mtime = d.mtime;
	return dir("index", Qindex, 8r444, 0, mtime);
}

dir(self: string, path, mode, length, mtime: int): Sys->Dir
{
	d := sys->zerodir;
	d.self = self;
	d.uid = d.gid = "llmfs";
	d.qid.path = big path;
	d.qid.qtype = Sys->QTFILE;
	if(mode&Sys->DMDIR)
		d.qid.qtype = Sys->QTDIR;
	d.mtime = d.atime = mtime;
	d.mode = mode;
	d.length = big length;
	return d;
}

strtabitems[T](t: ref Strhash[T]): list of ref (string, T)
{
	r: list of ref (string, T);
	for(i := 0; i < len t.items; i++)
		for(l := t.items[i]; l != nil; l = tl l)
			r = ref hd l::r;
	return r;
}

tabitems[T](t: ref Table[T]): list of ref (int, T)
{
	r: list of ref (int, T);
	for(i := 0; i < len t.items; i++)
		for(l := t.items[i]; l != nil; l = tl l)
			r = ref hd l::r;
	return r;
}

find(a: array of string, s: string): int
{
	for(i := 0; i < len a; i++)
		if(a[i] == s)
			return i;
	return -1;
}

suffix(suf, s: string): int
{
	return len s >= len suf && suf == s[len s-len suf:];
}

pid(): int
{
	return sys->pctl(0, nil);
}

killgrp(pid: int)
{
	sys->fprint(sys->open(sprint("/prog/%d/ctl", pid), Sys->OWRITE), "killgrp");
}

rev[T](l: list of T): list of T
{
	r: list of T;
	for(; l != nil; l = tl l)
		r = hd l::r;
	return r;
}

l2a[T](l: list of T): array of T
{
	a := array[len l] of T;
	i := 0;
	for(; l != nil; l = tl l)
		a[i++] = hd l;
	return a;
}

fd2: ref Sys->FD;
warn(s: string)
{
	if(fd2 == nil)
		fd2 = sys->fildes(2);
	sys->fprint(fd2, "%s\n", s);
}

say(s: string)
{
	if(dflag)
		warn(s);
}

fail(s: string)
{
	warn(s);
	killgrp(pid());
	raise "fail:"+s;
}

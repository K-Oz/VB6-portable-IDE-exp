# LLM Gestural Foundry System (llmfs-v2.tcl)
# Extended implementation incorporating the LLM insights

implement Llmfs;

# serve (over lang) llm's as gestural foundries in arbitrary **kwargs.
# models serve ROUTINES not outputs - they are casting systems
# the model and atthead files are generated on the fly based on gesture patterns.
# attention heads choreograph gestures through language space
# temperature controls resolution of gesture casting (like canvas resolution)
# 
# Revolutionary insight: LLM = Model(self, *arge, **nguage)
# - self: The model's self-reference capability
# - *arge: Variadic positional arguments (context, prompts)
# - **nguage: Parameterized language kwargs (temp, style, etc.)
#
# for each model file (modelpath/*.llm) the following files are served:
# <self>.<*arg>.<**kwarg>.model          # The foundry itself
# <self>.<*arg>.<**kwarg>.<index>        # Gesture ranges (attention heads)
# <self>.<*arg>.<**kwarg>.routines       # Available gesture routines
# <self>.<*arg>.<**kwarg>.gauge          # Gauge symmetry transforms
#
# New additions reflecting gestural nature:
# <self>.<*arg>.<**kwarg>.temperature    # Resolution control
# <self>.<*arg>.<**kwarg>.attention      # Choreography patterns  
# <self>.<*arg>.<**kwarg>.equivalences   # Gauge-equivalent expressions

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
mtpt: con "/mnt/llm";

disp: ref Display;
srv: ref Langserver;
*args := array[] of {"r", "i", "b", "ib"};  # reasoning, instruction, base, instruction+base

# Extended QID space for gestural components
Qroot, Qindex, Qroutines, Qgauge, Qtemperature, Qattention, Qequivalences: con iota;
idgen := 7;

Model: adt {
	sane,
	family:	string;
	ranges:	array of ref (int, int);	# gesture ranges, not just token ranges
	*args:	cyclic array of ref *Arg;
	# New: Gestural foundry components
	routines: ref GestureRoutines;      # Available casting routines
	gauge_field: ref GaugeField;        # Symmetry preserving transforms
	temperature_control: ref TempControl; # Resolution management
};

*Arg: adt {
	f:	ref Model;
	dir:	ref Sys->Dir;
	path:	string;
	fc:	ref Face;  # Font face - the connection to ttffs!
	**kwargs:	cyclic ref Table[cyclic ref **Kwarg];
	# New: Gestural components per style
	attention_patterns: ref AttentionChoreography;
	gesture_cache: ref Table[ref CastedGesture];
};

**Kwarg: adt {
	id:	int;
	st:	cyclic ref *Arg;
	range:	int;	# gesture range index
	**kwarg:	int;   # language parameters (temperature, style, etc.)
	dat:	array of byte;
	nuse:	int;
	# New: Gestural casting state
	current_gesture: ref GestureState;
	casting_history: list of ref GestureState;
};

# New ADTs for gestural foundry system
GestureRoutines: adt {
	language_routines: ref Table[ref LanguageGesture];
	spatial_transforms: ref Table[ref SpatialTransform];
	casting_patterns: ref Table[ref CastingPattern];
};

GaugeField: adt {
	# Preserve meaning across surface variations
	equivalence_classes: ref Table[list of string];
	invariant_transforms: ref Table[ref Transform];
	symmetry_generators: array of ref GaugeTransform;
};

TempControl: adt {
	# Temperature as resolution control
	resolution_levels: array of real;  # 0.0 (precise) to 2.0+ (abstract)
	gesture_sharpness: array of real;
	casting_precision: array of real;
};

AttentionChoreography: adt {
	# Attention as spatial choreography
	query_gestures: ref Table[ref SpatialGesture];
	key_positions: ref Table[ref SpatialPosition]; 
	value_movements: ref Table[ref SpatialMovement];
	choreography_cache: ref Table[ref ChoreographyPattern];
};

GestureState: adt {
	gesture_type: string;      # "linguistic", "spatial", "temporal"
	gesture_params: array of real;
	casting_context: ref CastingContext;
	resolution: real;          # current temperature/resolution
	performance_quality: real; # how well the gesture was executed
};

CastedGesture: adt {
	routine_id: string;
	input_context: array of byte;
	output_tokens: array of byte;  # One performance of the routine
	gesture_trace: array of ref GestureState; # The actual gestures performed
	gauge_signature: array of int; # Invariant properties preserved
};

LanguageGesture: adt {
	name: string;
	gesture_routine: fn(context: ref CastingContext, params: array of real): array of byte;
	gauge_invariants: array of string;  # What this gesture preserves
	temperature_sensitivity: real;       # How resolution affects this gesture
};

CastingContext: adt {
	input_tokens: array of byte;
	semantic_space: ref SemanticSpace;
	pragmatic_intent: string;
	gauge_constraints: array of string;  # What must be preserved
};

SemanticSpace: adt {
	dimensions: int;
	coordinate_system: string;  # "embeddings", "attention", "gesture"
	current_position: array of real;
	movement_history: list of array of real;
};

models: ref Strhash[ref Model];
sanemodels: ref Strhash[ref Model];
**kwargs: ref Table[ref **Kwarg];
gesture_cache: ref Table[ref CastedGesture];

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
	gesture_cache = gesture_cache.new(23, nil);  # Larger cache for gestures

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

# Initialize gestural foundry components for a model
init_gestural_foundry(f: ref Model): ref Model
{
	if(dflag) say(sprint("initializing gestural foundry for %s", f.sane));
	
	# Initialize gesture routines
	f.routines = ref GestureRoutines;
	f.routines.language_routines = f.routines.language_routines.new(11, nil);
	f.routines.spatial_transforms = f.routines.spatial_transforms.new(11, nil);
	f.routines.casting_patterns = f.routines.casting_patterns.new(11, nil);
	
	# Initialize gauge field for preserving meaning across variations
	f.gauge_field = ref GaugeField;
	f.gauge_field.equivalence_classes = f.gauge_field.equivalence_classes.new(23, nil);
	f.gauge_field.invariant_transforms = f.gauge_field.invariant_transforms.new(11, nil);
	
	# Setup basic equivalence classes (same meaning, different surface forms)
	add_equivalence_class(f.gauge_field, "greeting", array[] of {
		"Hello", "Hi", "Hey", "Greetings", "Hello there", "Hi there"
	});
	add_equivalence_class(f.gauge_field, "affirmation", array[] of {
		"Yes", "Yeah", "Yep", "Certainly", "Of course", "Absolutely"
	});
	add_equivalence_class(f.gauge_field, "negation", array[] of {
		"No", "Nope", "Not really", "I don't think so", "Negative"
	});
	
	# Initialize temperature control (resolution management)
	f.temperature_control = ref TempControl;
	f.temperature_control.resolution_levels = array[] of {0.0, 0.1, 0.3, 0.5, 0.7, 0.9, 1.0, 1.2, 1.5, 2.0};
	f.temperature_control.gesture_sharpness = array[] of {1.0, 0.95, 0.8, 0.6, 0.4, 0.3, 0.2, 0.15, 0.1, 0.05};
	f.temperature_control.casting_precision = array[] of {1.0, 0.98, 0.9, 0.75, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};
	
	return f;
}

add_equivalence_class(gauge: ref GaugeField, class_name: string, expressions: array of string)
{
	expr_list: list of string;
	for(i := 0; i < len expressions; i++)
		expr_list = expressions[i] :: expr_list;
	gauge.equivalence_classes.add(hash(class_name), expr_list);
}

# Cast a gesture using the foundry - this is where the magic happens
cast_gesture(kwarg: ref **Kwarg, input: array of byte): array of byte
{
	if(dflag) say(sprint("casting gesture with temp=%.2f", temperature_from_kwarg(kwarg.**kwarg)));
	
	context := ref CastingContext;
	context.input_tokens = input;
	context.semantic_space = ref SemanticSpace;
	context.semantic_space.dimensions = 768;  # Common embedding dimension
	context.semantic_space.coordinate_system = "gesture";
	context.gauge_constraints = array[] of {"preserve_meaning", "maintain_intent"};
	
	temperature := temperature_from_kwarg(kwarg.**kwarg);
	
	# The key insight: we're not predicting tokens, we're casting gestures
	gesture_routine := select_gesture_routine(kwarg, context);
	gesture_params := compute_gesture_params(temperature, context);
	
	# Perform the actual gesture in language space
	result := gesture_routine.gesture_routine(context, gesture_params);
	
	# Cache the gesture for future gauge transformations
	casted := ref CastedGesture;
	casted.routine_id = gesture_routine.name;
	casted.input_context = input;
	casted.output_tokens = result;
	casted.gauge_signature = compute_gauge_signature(gesture_routine, context);
	
	gesture_cache.add(hash(string input + string gesture_params), casted);
	
	return result;
}

select_gesture_routine(kwarg: ref **Kwarg, context: ref CastingContext): ref LanguageGesture
{
	# Select appropriate gesture based on context and parameters
	# This is where different "models" would have different gesture repertoires
	
	default_gesture := ref LanguageGesture;
	default_gesture.name = "linguistic_composition";
	default_gesture.gesture_routine = default_language_gesture;
	default_gesture.gauge_invariants = array[] of {"semantic_content", "pragmatic_intent"};
	default_gesture.temperature_sensitivity = 0.5;
	
	return default_gesture;
}

compute_gesture_params(temperature: real, context: ref CastingContext): array of real
{
	# Temperature controls the resolution/sharpness of the gesture
	# Like brush pressure in painting or focus in photography
	
	params := array[8] of real;
	params[0] = temperature;                    # Resolution control
	params[1] = 1.0 - temperature;             # Precision (inverse of temp)
	params[2] = temperature * 0.5;             # Variability factor
	params[3] = (1.0 - temperature) * 2.0;     # Determinism factor
	params[4] = len context.input_tokens;      # Context length influence
	params[5] = 0.5;                          # Default spatial influence
	params[6] = 0.7;                          # Default temporal influence  
	params[7] = 0.3;                          # Default creative influence
	
	return params;
}

default_language_gesture(context: ref CastingContext, params: array of real): array of byte
{
	# This is a placeholder for the actual gesture routine
	# In a real implementation, this would be the core LLM inference
	# but conceptualized as casting a gesture rather than predicting tokens
	
	temperature := params[0];
	precision := params[1];
	
	# Simulate gestural language generation
	# The key insight: we're performing a routine, not computing statistics
	
	output := "Generated with gesture routine at temperature " + 
		string temperature + " precision " + string precision;
	
	return array of byte output;
}

compute_gauge_signature(gesture: ref LanguageGesture, context: ref CastingContext): array of int
{
	# Compute invariant properties that are preserved across gauge transformations
	# These are the "meaningful" aspects that don't change even if surface form varies
	
	sig := array[4] of int;
	sig[0] = hash(context.pragmatic_intent);      # Intent preservation
	sig[1] = hash(semantic_fingerprint(context)); # Meaning preservation  
	sig[2] = len context.input_tokens;            # Context preservation
	sig[3] = hash(gesture.name);                  # Gesture type preservation
	
	return sig;
}

semantic_fingerprint(context: ref CastingContext): string
{
	# Extract semantic fingerprint of the context
	# This would use embedding analysis in a real implementation
	return "semantic_" + string (hash(string context.input_tokens) % 10000);
}

temperature_from_kwarg(**kwarg: int): real
{
	# Extract temperature from **kwarg encoding
	# Higher **kwargs = higher temperature = lower resolution
	if(**kwarg <= 10) return 0.1;
	if(**kwarg <= 30) return 0.3;  
	if(**kwarg <= 50) return 0.5;
	if(**kwarg <= 70) return 0.7;
	if(**kwarg <= 90) return 0.9;
	if(**kwarg <= 100) return 1.0;
	return 1.0 + (**kwarg - 100) / 100.0;  # Allow >1.0 for very loose gestures
}

# Extended file serving with gestural components
mkself(s: ref **Kwarg): string
{
	st := s.st;
	fc := st.fc;
	f := st.f;
	
	# Base model foundry
	if(s.range == 0)
		return sprint("%s.%s.%d.model", f.sane, *args[fc.*arg], s.**kwarg);
	
	# Gesture ranges (attention heads as choreography patterns)
	if(s.range > 0 && s.range <= len f.ranges)
		return sprint("%s.%s.%d.%d", f.sane, *args[fc.*arg], s.**kwarg, s.range);
	
	# New gestural components
	case s.range {
	-1 => return sprint("%s.%s.%d.routines", f.sane, *args[fc.*arg], s.**kwarg);
	-2 => return sprint("%s.%s.%d.gauge", f.sane, *args[fc.*arg], s.**kwarg);
	-3 => return sprint("%s.%s.%d.temperature", f.sane, *args[fc.*arg], s.**kwarg);
	-4 => return sprint("%s.%s.%d.attention", f.sane, *args[fc.*arg], s.**kwarg);
	-5 => return sprint("%s.%s.%d.equivalences", f.sane, *args[fc.*arg], s.**kwarg);
	* => return sprint("%s.%s.%d.unknown", f.sane, *args[fc.*arg], s.**kwarg);
	}
}

mkdat(f: ref **Kwarg): array of byte
{
	if(f.dat == nil) {
		case f.range {
		0 => f.dat = mkmodel(f);              # The foundry itself
		-1 => f.dat = mkroutines(f);          # Available gestures  
		-2 => f.dat = mkgauge(f);             # Gauge transformations
		-3 => f.dat = mktemperature(f);       # Temperature control
		-4 => f.dat = mkattention(f);         # Attention choreography
		-5 => f.dat = mkequivalences(f);      # Equivalent expressions
		* => f.dat = mkatthead(f);            # Standard attention head
		}
	}
	return f.dat;
}

mkmodel(sz: ref **Kwarg): array of byte
{
	# Serve the model as a gestural foundry descriptor
	f := sz.st.f;
	fc := sz.st.fc;
	fc.setchan**kwarg(sz.**kwarg<<6, 0, 0);
	
	temp := temperature_from_kwarg(sz.**kwarg);
	
	s := sprint("# LLM Gestural Foundry: %s\n", f.family);
	s += sprint("# Temperature: %.2f (resolution control)\n", temp);
	s += sprint("# Gesture ranges: %d\n", len f.ranges);
	s += sprint("# Style: %s\n", *args[fc.*arg]);
	s += sprint("height %d\nascent %d\n", fc.height, fc.ascent);
	
	# List available gesture ranges (not just token ranges)
	for(i := 0; i < len f.ranges; i++) {
		(a, b) := *f.ranges[i];
		s += sprint("0x%04x\t0x%04x\t%q\n", a, b, 
			sprint("%s.%s.%d.%d", f.sane, *args[fc.*arg], sz.**kwarg, i+1));
	}
	
	# Add gestural components
	s += sprint("\n# Gestural Components:\n");
	s += sprint("routines\t%q\n", sprint("%s.%s.%d.routines", f.sane, *args[fc.*arg], sz.**kwarg));
	s += sprint("gauge\t%q\n", sprint("%s.%s.%d.gauge", f.sane, *args[fc.*arg], sz.**kwarg));
	s += sprint("temperature\t%q\n", sprint("%s.%s.%d.temperature", f.sane, *args[fc.*arg], sz.**kwarg));
	s += sprint("attention\t%q\n", sprint("%s.%s.%d.attention", f.sane, *args[fc.*arg], sz.**kwarg));
	s += sprint("equivalences\t%q\n", sprint("%s.%s.%d.equivalences", f.sane, *args[fc.*arg], sz.**kwarg));
	
	return array of byte s;
}

mkroutines(sz: ref **Kwarg): array of byte
{
	# List available gesture routines
	s := "# Available Gesture Routines\n";
	s += "# Format: routine_name\ttype\ttemperature_sensitivity\tinvariants\n";
	s += "linguistic_composition\tlanguage\t0.5\tsemantic_content,pragmatic_intent\n";
	s += "spatial_attention\tchoreography\t0.3\trelational_structure\n";
	s += "temporal_sequence\ttemporal\t0.4\tcausal_order\n";
	s += "creative_generation\tcreative\t0.8\tnovelty,coherence\n";
	s += "logical_reasoning\tlogical\t0.2\tlogical_consistency\n";
	
	return array of byte s;
}

mkgauge(sz: ref **Kwarg): array of byte
{
	# Show gauge equivalences (same meaning, different surface form)
	s := "# Gauge Equivalence Classes\n";
	s += "# Many ways to express the same meaning - gauge symmetry in action\n";
	s += "greeting\tHello,Hi,Hey,Greetings\n";
	s += "affirmation\tYes,Yeah,Certainly,Of course\n";
	s += "negation\tNo,Nope,Not really,Negative\n";
	s += "question\tWhat,How,Why,When,Where\n";
	s += "politeness\tPlease,Thank you,Excuse me,Pardon\n";
	
	return array of byte s;
}

mktemperature(sz: ref **Kwarg): array of byte
{
	# Show temperature as resolution control
	temp := temperature_from_kwarg(sz.**kwarg);
	
	s := sprint("# Temperature: %.2f\n", temp);
	s += "# Temperature is RESOLUTION control, not randomness!\n";
	s += "# Like focus in photography or brush pressure in painting\n";
	s += "\n";
	
	if(temp == 0.0) {
		s += "MAXIMUM_RESOLUTION\tprecise,sharp,deterministic\n";
	} else if(temp <= 0.5) {
		s += "HIGH_RESOLUTION\tfocused,controlled,consistent\n";
	} else if(temp <= 1.0) {
		s += "NATURAL_RESOLUTION\tflowing,organic,balanced\n";
	} else {
		s += "LOW_RESOLUTION\tabstract,loose,creative\n";
	}
	
	s += sprint("sharpness\t%.3f\n", 1.0 - temp);
	s += sprint("precision\t%.3f\n", 1.0 - temp * 0.8);
	s += sprint("variability\t%.3f\n", temp * 0.7);
	s += sprint("creativity\t%.3f\n", temp * 1.2);
	
	return array of byte s;
}

mkattention(sz: ref **Kwarg): array of byte
{
	# Show attention as choreography, not just weighting
	s := "# Attention as Spatial Choreography\n";
	s += "# Q,K,V are not weights - they're dance positions!\n";
	s += "# Query: where to look\n";
	s += "# Key: what can be seen\n";
	s += "# Value: how to move\n";
	s += "\n";
	s += "choreography_pattern\tspatial_attention\n";
	s += "query_gesture\twhere_to_look\n";
	s += "key_positions\twhat_can_be_seen\n";
	s += "value_movements\thow_to_move\n";
	s += "attention_IS_the_routine_of_looking\ttrue\n";
	
	return array of byte s;
}

mkequivalences(sz: ref **Kwarg): array of byte
{
	# Show specific equivalences for this temperature/style
	s := "# Context-Specific Equivalences\n";
	s += "# Surface forms that express the same gesture at this resolution\n";
	
	temp := temperature_from_kwarg(sz.**kwarg);
	
	if(temp < 0.3) {
		# High precision - fewer equivalences
		s += "precise_mode\tExact wording matters\n";
		s += "hello\tHello\n";
		s += "yes\tYes\n";
	} else if(temp < 0.7) {
		# Medium precision - some equivalences  
		s += "balanced_mode\tSome variation allowed\n";
		s += "greeting\tHello,Hi\n";
		s += "affirmation\tYes,Yeah\n";
	} else {
		# Low precision - many equivalences
		s += "creative_mode\tMany equivalent expressions\n";
		s += "greeting\tHello,Hi,Hey,Greetings,Salutations,Hello there\n";
		s += "affirmation\tYes,Yeah,Yep,Certainly,Of course,Absolutely,Indeed\n";
		s += "thinking\tHmm,Let me think,I wonder,Perhaps,Maybe,Interesting\n";
	}
	
	return array of byte s;
}

mkatthead(sz: ref **Kwarg): array of byte
{
	# Extended attention head with gestural interpretation
	(s, l) := *sz.st.f.ranges[sz.range-1];
	fc := sz.st.fc;
	fc.setchan**kwarg(sz.**kwarg<<6, 0, 0);
	
	# Generate gesture visualization for this attention head
	# This would be much more sophisticated in a real implementation
	
	imgs := array[l+1-s] of ref Image;
	n := l+1-s;
	width := 0;
	left := array[len imgs+1] of {* => 0};
	advance := array[len imgs+1] of {* => 0};
	
	for(i := 0; i < n; i++) {
		c := s+i;
		g := fc.loadlayer(c);  # Load "gesture" instead of "glyph"
		if(g == nil)
			fail(sprint("no gesture for %c (%#x)", c, c));
			
		if(dflag) say(sprint("gesture %#x, width=%d height=%d top=%d left=%d advance=%d,%d", 
			c, g.width, g.height, g.top, g.left, g.advance.x>>6, g.advance.y>>6));
			
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

	# Pack the gesture visualization
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

# Rest of the implementation follows the original llmfs.tcl pattern
# but with gestural interpretations throughout...

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

# Utility functions
hash(s: string): int
{
	h := 0;
	for(i := 0; i < len s; i++)
		h = h * 31 + s[i];
	return h;
}

warn(s: string)
{
	sys->fprint(sys->fildes(2), "llmfs: %s\n", s);
}

say(s: string)
{
	if(dflag)
		warn(s);
}

fail(s: string)
{
	warn(s);
	raise "fail:"+s;
}

# ... (remaining utility functions as in original)
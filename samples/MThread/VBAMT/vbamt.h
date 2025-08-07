/*	Copyright (C) Microsoft Corporation, 1999.  All rights reserved.


	This source code is intended only as a supplement to Microsoft
	Development Tools and/or on-line documentation.  See these other
	materials for detailed information regarding Microsoft code samples.
*/

#ifndef _VBAMT_H_
#define _VBAMT_H_

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  INCLUDES
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "objbase.h"
#include "vbamt_i.h"
#include "vbamti.h"

#define UNUSED(x) x=x
DWORD WINAPI Mt1ThreadFunc( LPVOID );
DWORD WINAPI Mt2ThreadFunc( LPVOID );
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     Class:	CMTObj1
//
//  Abstract:   Object to run against a MTDLL
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CMTObj1 : public IMTObj1
{
public:
  friend DWORD WINAPI Mt1ThreadFunc( LPVOID );

  CMTObj1();
  ~CMTObj1();

    // *** IUnknown methods ***
    STDMETHOD(QueryInterface)(REFIID riid, LPVOID FAR* ppvObj);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // *** IDispatch methods ***
    STDMETHOD(GetTypeInfoCount)(UINT FAR* pctinfo);
    STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
    STDMETHOD(GetIDsOfNames)( REFIID riid, OLECHAR **rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid );
    STDMETHOD(Invoke)(DISPID dispidMember,
		      REFIID riid,
		      LCID lcid,
		      WORD wFlags,
		      DISPPARAMS FAR* pdispparams,
		      VARIANT FAR* pvarResult,
		      EXCEPINFO FAR* pexcepinfo,
		      UINT FAR* puArgErr);

    // *** IMTObj1 methods ***
    STDMETHOD(get_Prop1)(VARIANT* pVar);
    STDMETHOD(put_Prop1)(VARIANT var);

private:

    UINT	m_cRef;
    VARIANT	  m_var;
    ITypeInfo	*m_ptinfoDual;	// a typeinfo for our default interface which we'll use to implement idispatch
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     Class:	CMTObj2
//
//  Abstract:   Object to run against a MTDLL
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CMTObj2 : public IMTObj2
{
public:

	friend DWORD WINAPI Mt2ThreadFunc( LPVOID );

    CMTObj2();
    ~CMTObj2();

    // *** IUnknown methods ***
    STDMETHOD(QueryInterface)(REFIID riid, LPVOID FAR* ppvObj);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // *** IDispatch methods ***
    STDMETHOD(GetTypeInfoCount)(UINT FAR* pctinfo);
    STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
    STDMETHOD(GetIDsOfNames)( REFIID riid, OLECHAR **rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid );
    STDMETHOD(Invoke)(DISPID dispidMember,
		      REFIID riid,
		      LCID lcid,
		      WORD wFlags,
		      DISPPARAMS FAR* pdispparams,
		      VARIANT FAR* pvarResult,
		      EXCEPINFO FAR* pexcepinfo,
		      UINT FAR* puArgErr);

    // *** IMTObj2 methods ***
    STDMETHOD(get_Prop2)(VARIANT* pVar);
    STDMETHOD(put_Prop2)(VARIANT Bogus);

private:

    UINT	m_cRef;
    VARIANT	  m_var;
    ITypeInfo	*m_ptinfoDual;	// a typeinfo for our default interface which we'll use to implement idispatch
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     Class:	CVbaMt
//
//  Abstract:   MTDLL Test application base class
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CVbaMt
{
public:

  virtual void Start();
  virtual void Stop();
  virtual HANDLE Destroy();
  DWORD GetThreadID() { return m_dwThreadID; }
  BOOL IsStarted() { return m_fStarted; }

  CVbaMt() { m_fStarted = FALSE; }
  virtual ~CVbaMt() {}

protected:
  
  HANDLE m_hThread;
  DWORD m_dwThreadID;
  BOOL m_fStarted;

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     Class:	CVbaMt1
//
//  Abstract:   MTDLL Test application class
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CVbaMt1 : public CVbaMt
{
public:
  static CVbaMt* Create();
  
  CVbaMt1();
  virtual ~CVbaMt1();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     Class:	CVbaMt2
//
//  Abstract:   MTDLL Test application class
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CVbaMt2 : public CVbaMt
{
public:
  static CVbaMt* Create();

  CVbaMt2();
  virtual ~CVbaMt2();
};
#endif // _VBAMT_H_

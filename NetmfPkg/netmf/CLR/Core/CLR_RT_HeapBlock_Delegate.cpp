////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Core.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

HRESULT CLR_RT_HeapBlock_Delegate::CreateInstance( CLR_RT_HeapBlock& reference, const CLR_RT_MethodDef_Index& ftn, CLR_RT_StackFrame* call )
{
    NATIVE_PROFILE_CLR_CORE();
    TINYCLR_HEADER();

//    CLR_Debug::Printf( "  reference.SetObjectReference\r\n" );
    reference.SetObjectReference( NULL );

    CLR_UINT32 length = 0;

#if defined(TINYCLR_DELEGATE_PRESERVE_STACK)    
    if(call)
    {
        TINYCLR_FOREACH_NODE_BACKWARD__DIRECT(CLR_RT_StackFrame,ptr,call)
        {
            length++;
        }
        TINYCLR_FOREACH_NODE_BACKWARD_END();
    }

    //
    // Limit depth to three callers.
    //
    if(length > 3) length = 3;
#endif

    CLR_UINT32 totLength = (CLR_UINT32)(sizeof(CLR_RT_HeapBlock_Delegate) + length * sizeof(CLR_RT_MethodDef_Index));

//    CLR_Debug::Printf( "  g_CLR_RT_ExecutionEngine.ExtractHeapBytesForObjects\r\n" );
    CLR_RT_HeapBlock_Delegate* dlg = (CLR_RT_HeapBlock_Delegate*)g_CLR_RT_ExecutionEngine.ExtractHeapBytesForObjects( DATATYPE_DELEGATE_HEAD, 0, totLength ); CHECK_ALLOCATION(dlg);

//    CLR_Debug::Printf( "  reference.SetObjectReference(dlg)\r\n" );
    reference.SetObjectReference( dlg );

    dlg->ClearData();
    dlg->m_cls.Clear();
    dlg->m_ftn              = ftn;
#if defined(TINYCLR_DELEGATE_PRESERVE_STACK)
    dlg->m_numOfStackFrames = length;
#endif
    
//    CLR_Debug::Printf( "  dlg->m_object.SetObjectReference\r\n" );
    dlg->m_object.SetObjectReference( NULL );

#if defined(TINYCLR_APPDOMAINS)
//    CLR_Debug::Printf( "  g_CLR_RT_ExecutionEngine.GetCurrentAppDomain\r\n" );
    dlg->m_appDomain = g_CLR_RT_ExecutionEngine.GetCurrentAppDomain();
//    CLR_Debug::Printf( "  dlg->m_appDomain - 0x%x\r\n", dlg->m_appDomain );
#endif

#if defined(TINYCLR_DELEGATE_PRESERVE_STACK)
    if(call)
    {
        CLR_RT_MethodDef_Index* callStack = dlg->GetStackFrames();

        TINYCLR_FOREACH_NODE_BACKWARD__DIRECT(CLR_RT_StackFrame,ptr,call)
        {
            if(length-- == 0) break;

            *callStack++ = ptr->m_call;
        }
        TINYCLR_FOREACH_NODE_BACKWARD_END();
    }
#endif

    TINYCLR_NOCLEANUP();
}

void CLR_RT_HeapBlock_Delegate::Relocate()
{
    NATIVE_PROFILE_CLR_CORE();
    m_object.Relocate__HeapBlock();
}

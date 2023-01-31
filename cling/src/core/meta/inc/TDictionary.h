// @(#)root/meta:$Id$
// Author: Fons Rademakers   20/06/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TDictionary
#define ROOT_TDictionary

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TDictionary                                                          //
//                                                                      //
// This class defines an abstract interface that must be implemented    //
// by all classes that contain dictionary information.                  //
//                                                                      //
// The dictionary is defined by the followling classes:                 //
// TDataType                              (typedef definitions)         //
// TGlobal                                (global variables)            //
// TFunction                              (global functions)            //
// TClass                                 (classes)                     //
//    TBaseClass                          (base classes)                //
//    TDataMember                         (class datamembers)           //
//    TMethod                             (class methods)               //
//       TMethodArg                       (method arguments)            //
//                                                                      //
// All the above classes implement the TDictionary abstract interface   //
// (note: the indentation shows aggregation not inheritance).           //
// The ROOT dictionary system provides a very extensive RTTI            //
// environment that facilitates a.o. object inspectors, object I/O,     //
// ROOT Trees, etc. Most of the type information is provided by the     //
// CINT C++ interpreter.                                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "ESTLType.h"


namespace CppyyLegacy {

// The following are opaque type and are never really declared
// The specific implementation of TInterpreter will cast the
// value of pointer to this types to the correct type (possibly
// distinct from these)
class CallFunc_t;
class DataMemberInfo_t;
class FuncTempInfo_t;
class MethodInfo_t;
class MethodArgInfo_t;
class MethodArgInfo_t;
class TypeInfo_t;
class TypedefInfo_t;
class TDictAttributeMap;

enum EProperty {
   kIsClass         = 0x00000001,
   kIsStruct        = 0x00000002,
   kIsUnion         = 0x00000004,
   kIsEnum          = 0x00000008,
   kIsTypedef       = 0x00000010,
   kIsFundamental   = 0x00000020,
   kIsAbstract      = 0x00000040,
   kIsVirtual       = 0x00000080,
   kIsPureVirtual   = 0x00000100,
   kIsPublic        = 0x00000200,
   kIsProtected     = 0x00000400,
   kIsPrivate       = 0x00000800,
   kIsPointer       = 0x00001000,
   kIsArray         = 0x00002000,
   kIsStatic        = 0x00004000,
   kIsDefault       = 0x00008000,
   kIsReference     = 0x00010000,
   kIsDirectInherit = 0x00020000,
   kIsCCompiled     = 0x00040000,
   kIsCPPCompiled   = kIsCCompiled,
   kIsCompiled      = kIsCCompiled,
   kIsConstant      = 0x00100000,
   kIsVirtualBase   = 0x00200000,
   kIsConstPointer  = 0x00400000,
   kIsScopedEnum    = 0x00800000,
   kIsConstexpr     = 0x02000000,
   kIsExplicit      = 0x04000000,
   kIsNamespace     = 0x08000000,
   kIsConstMethod   = 0x10000000,
   kIsUsingVariable = 0x20000000,
   kIsDefinedInStd  = 0x40000000
};

enum EFunctionProperty {
/* The following are already defined
   in EProperty and 'could' be part of this enums */
/*
   kIsAbstract      = 0x00000040,
   kIsVirtual       = 0x00000080,
   kIsPureVirtual   = 0x00000100,
   kIsPublic        = 0x00000200,
   kIsProtected     = 0x00000400,
   kIsPrivate       = 0x00000800,
   kIsStatic        = 0x00004000,
   kIsDefault       = 0x00008000,
   kIsConstexpr     = 0x02000000
   kIsExplicit      = 0x04000000,
   kIsConstMethod   = 0x10000000,
*/
/* The following are already defined
   in EProperty and related to the type of the return value */
/*
 kIsConstant      = 0x00100000,
 kIsPointer       = 0x00001000,
 kIsArray         = 0x00002000,
 kIsReference     = 0x00010000,
 kIsConstPointer  = 0x00400000,
*/
   kIsConstructor = 0x00000001,
   kIsConversion  = 0x00000002,
   kIsDestructor  = 0x00000004,
   kIsOperator    = 0x00000008,
   kIsInlined     = 0x00000010,
   kIsTemplateSpec= 0x00000020
};

enum EClassProperty {
   kClassIsValid         = 0x00000001,
   kClassHasExplicitCtor = 0x00000010,
   kClassHasImplicitCtor = 0x00000020,
   kClassHasCtor         = 0x00000030,
   kClassHasDefaultCtor  = 0x00000040,
   kClassHasAssignOpr    = 0x00000080,
   kClassHasExplicitDtor = 0x00000100,
   kClassHasImplicitDtor = 0x00000200,
   kClassHasDtor         = 0x00000300,
   kClassHasVirtual      = 0x00001000,
   kClassIsAbstract      = 0x00002000,
   kClassIsAggregate     = 0x00004000
};

enum ERefTypeValues {
   kParaNormal     = 0,     // not used
   kParaReference  = 1,
   kParaP2P        = 2,     // not used
   kParaP2P2P      = 3,     // not used
   kParaRef        = 100,
   kParaRefP2P     = 102,   // not used
   kParaRefP2P2P   = 103    // not used
};

enum EFunctionMatchMode {
   kExactMatch = 0,
   kConversionMatch = 1
};


class TDictionary : public TNamed {

private:
   TDictAttributeMap *fAttributeMap;    //pointer to a class attribute map
   ULong64_t fUpdatingTransactionCount; //!the Cling ID of the transaction that last updated the object

protected:
   Bool_t              UpdateInterpreterStateMarker();

public:
   TDictionary(): fAttributeMap(0), fUpdatingTransactionCount(0) { }
   TDictionary(const char* name): TNamed(name, ""), fAttributeMap(0), fUpdatingTransactionCount(0) { }
   TDictionary(const TDictionary& dict);
   virtual ~TDictionary();

   TDictionary& operator=(const TDictionary& other);

   void                CreateAttributeMap();
   TDictAttributeMap  *GetAttributeMap() const
   {
      //Get the TDictAttributeMap pointer to be able to add attribute
      //pairs key-value to the TClass.

      return fAttributeMap;
   }
   virtual Long_t      Property() const = 0;
   static TDictionary* GetDictionary(const char* name);
   static TDictionary* GetDictionary(const std::type_info &typeinfo);

   // Type of STL container (returned by IsSTLContainer).
   enum ESTLType {
      kNone              = CppyyLegacy::kNotSTL,
      kVector            = CppyyLegacy::kSTLvector,
      kList              = CppyyLegacy::kSTLlist,
      kForwardlist       = CppyyLegacy::kSTLforwardlist,
      kDeque             = CppyyLegacy::kSTLdeque,
      kMap               = CppyyLegacy::kSTLmap,
      kMultimap          = CppyyLegacy::kSTLmultimap,
      kSet               = CppyyLegacy::kSTLset,
      kMultiset          = CppyyLegacy::kSTLmultiset,
      kUnorderedSet      = CppyyLegacy::kSTLunorderedset,
      kUnorderedMultiset = CppyyLegacy::kSTLunorderedmultiset,
      kUnorderedMap      = CppyyLegacy::kSTLunorderedmap,
      kUnorderedMultimap = CppyyLegacy::kSTLunorderedmultimap,
      kBitset            = CppyyLegacy::kSTLbitset
   };

   typedef const void *DeclId_t;
   ClassDef(TDictionary,2)  //Interface to dictionary
};

} // namespace CppyyLegacy

#endif

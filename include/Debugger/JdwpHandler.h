//===- JdwpHandler.h - ART-DEBUGGER ------------------------------*- C++ -*-===//
//
//                     ANDROID REVERSE TOOLKIT
//
// This file is distributed under the GNU GENERAL PUBLIC LICENSE
// V3 License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// JdwpHandler is used to handle jdwp request.
//
//===----------------------------------------------------------------------===//


#ifndef PROJECT_JDWPHANDLER_H
#define PROJECT_JDWPHANDLER_H

#include "JdwpReader.h"
/* JDWP Command
 * cmd  cmdset  name
 *
 * VirtualMachine command set (1)
 * 1    1       VirtualMachine.Version
 * 1    2       VirtualMachine.ClassesBySignature
 * 1    3       VirtualMachine.AllClasses
 * 1    4       VirtualMachine.AllThreads
 * 1    5       VirtualMachine.TopLevelThreadGroups
 * 1    6       VirtualMachine.Dispose
 * 1    7       VirtualMachine.IDSizes
 * 1    8       VirtualMachine.Suspend
 * 1    9       VirtualMachine.Resume
 * 1    10      VirtualMachine.Exit
 * 1    11      VirtualMachine.CreateString
 * 1    12      VirtualMachine.Capabilities
 * 1    13      VirtualMachine.ClassPaths
 * 1    14      VirtualMachine.DisposeObjects
 * 1    15      VirtualMachine.HoldEvents
 * 1    16      VirtualMachine.ReleaseEvents
 * 1    17      VirtualMachine.CapabilitiesNew
 * 1    18      VirtualMachine.RedefineClasses
 * 1    19      VirtualMachine.SetDefaultStratum
 * 1    20      VirtualMachine.AllClassesWithGeneric
 * 1    21      VirtualMachine.InstanceCounts
 *
 * ReferenceType command set (2)
 * 2    1       ReferenceType.Signature
 * 2    2       ReferenceType.ClassLoader
 * 2    3       ReferenceType.Modifiers
 * 2    4       ReferenceType.Fields
 * 2    5       ReferenceType.Methods
 * 2    6       ReferenceType.GetValues
 * 2    7       ReferenceType.SourceFile
 * 2    8       ReferenceType.NestedTypes
 * 2    9       ReferenceType.Status
 * 2    10      ReferenceType.Interfaces
 * 2    11      ReferenceType.ClassObject
 * 2    12      ReferenceType.SourceDebugExtension
 * 2    13      ReferenceType.SignatureWithGeneric
 * 2    14      ReferenceType.FieldsWithGeneric
 * 2    15      ReferenceType.MethodsWithGeneric
 * 2    16      ReferenceType.Instances
 * 2    17      ReferenceType.ClassFileVersion
 * 2    18      ReferenceType.ConstantPool
 *
 * ClassType command set (3)
 * 3    1       ClassType.Superclass
 * 3    2       ClassType.SetValues
 * 3    3       ClassType.InvokeMethod
 * 3    4       ClassType.NewInstance
 *
 * ArrayType command set (4)
 * 4    1       ArrayType.NewInstance
 *
 * InterfaceType command set (5)
 *
 * Method command set (6)
 * 6    1       Method.LineTable
 * 6    2       Method.VariableTable
 * 6    3       Method.Bytecodes
 * 6    4       Method.IsObsolete
 * 6    5       Method.VariableTableWithGeneric
 *
 * Field command set (8)
 *
 * ObjectReference command set (9)
 * 9    1       ObjectReference.ReferenceType
 * 9    2       ObjectReference.GetValues
 * 9    3       ObjectReference.SetValues
 * 9    4       ObjectReference.UNUSED
 * 9    5       ObjectReference.MonitorInfo
 * 9    6       ObjectReference.InvokeMethod
 * 9    7       ObjectReference.DisableCollection
 * 9    8       ObjectReference.EnableCollection
 * 9    9       ObjectReference.IsCollected
 * 9    10      ObjectReference.ReferringObjects
 *
 * StringReference command set (10)
 * 10   1       StringReference.Value
 *
 * ThreadReference command set (11)
 * 11   1       ThreadReference.Name
 * 11   2       ThreadReference.Suspend
 * 11   3       ThreadReference.Resume
 * 11   4       ThreadReference.Status
 * 11   5       ThreadReference.ThreadGroup
 * 11   6       ThreadReference.Frames
 * 11   7       ThreadReference.FrameCount
 * 11   8       ThreadReference.OwnedMonitors
 * 11   9       ThreadReference.CurrentContendedMonitor
 * 11   10      ThreadReference.Stop
 * 11   11      ThreadReference.Interrupt
 * 11   12      ThreadReference.SuspendCount
 * 11   13      ThreadReference.OwnedMonitorsStackDepthInfo
 * 11   14      ThreadReference.ForceEarlyReturn
 *
 * ThreadGroupReference command set (12)
 * 12   1       ThreadGroupReference.Name
 * 12   2       ThreadGroupReference.Parent
 * 12   3       ThreadGroupReference.Children
 *
 * ArrayReference command set (13)
 * 13   1       ArrayReference.Length
 * 13   2       ArrayReference.GetValues
 * 13   3       ArrayReference.SetValues
 *
 * ClassLoaderReference command set (14)
 * 14   1       ClassLoaderReference.VisibleClasses
 *
 * EventRequest command set (15)
 * 15   1       EventRequest.Set
 * 15   2       EventRequest.Clear
 * 15   3       EventRequest.ClearAllBreakpoints
 *
 * StackFrame command set (16)
 * 16   1       StackFrame.GetValues
 * 16   2       StackFrame.SetValues
 * 16   3       StackFrame.ThisObject
 * 16   4       StackFrame.PopFrames
 *
 * ClassObjectReference command set (17)
 * 17   1       ClassObjectReference.ReflectedType
 *
 * Event command set (64)
 * 64   100     Event.Composite
 *
 * 199  1       DDM.Chunk
 * */

#include "JdwpReader.h"
#include "jdwp.h"
#include <vector>

namespace JDWP {
    namespace VirtualMachine {
        uint8_t set_ = 1;

        struct Version : public JdwpReader {
            Version(const uint8_t* bytes, uint32_t available);

            std::string version;
            uint32_t major;
            uint32_t minor;
            std::string javaVersion;
            std::string javaVmName;

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 1;
        };

        struct ClassesBySignature : public JdwpReader {
            ClassesBySignature(const uint8_t* bytes, uint32_t available);

            uint32_t mSize;
            std::vector<ClassInfo> mInfos;

            static std::string buildReq(const std::string &classDescriptor,
                                        int id = 0);
            const static uint8_t cmd = 2;
        };

        struct AllClasses : public JdwpReader {
            AllClasses(const uint8_t* bytes, uint32_t available);

            uint32_t mSize;
            std::vector<ClassInfo> mInfos;

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 3;
        };

        struct AllThreads : public JdwpReader {
            AllThreads(const uint8_t* bytes, uint32_t available);
            uint32_t mSize;
            std::vector<ObjectId> mThreadIds;

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 4;
        };

        struct TopLevelThreadGroups : public JdwpReader {
            TopLevelThreadGroups(const uint8_t* bytes, uint32_t available);

            uint32_t mGroups;
            ObjectId mThreadGroupId;

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 5;
        };

        struct Dispose : public JdwpReader {
            Dispose(const uint8_t* bytes, uint32_t available);


            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 6;
        };

        struct IDSizes : public JdwpReader {
            IDSizes(const uint8_t* bytes, uint32_t available);

            uint32_t mFieldIdSize;
            uint32_t mMethodIdSize;
            uint32_t mObjectIdSize;
            uint32_t mRefTypeIdSize;
            uint32_t mFrameIdSize;

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 7;
        };

        struct Suspend : public JdwpReader {
            Suspend(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 8;
        };

        struct Resume : public JdwpReader {
            Resume(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 9;
        };

        struct Exit : public JdwpReader {
            Exit(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 10;
        };

        struct CreateString : public JdwpReader {
            CreateString(const uint8_t* bytes, uint32_t available);
            ObjectId mStringId;

            static std::string buildReq(const std::string str, int id = 0);
            const static uint8_t cmd = 11;
        };

        struct Capabilities : public JdwpReader {
            Capabilities(const uint8_t* bytes, uint32_t available);

            uint8_t mCanWatchFieldModification;
            uint8_t mCanWatchFieldAccess;
            uint8_t mCanGetBytecodes;
            uint8_t mCanGetSyntheticAttribute;
            uint8_t mCanGetOwnedMonitorInfo;
            uint8_t mCanGetCurrentContendedMonitor;
            uint8_t mCanGetMonitorInfo;

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 12;
        };

        struct ClassPaths : public JdwpReader {
            ClassPaths(const uint8_t* bytes, uint32_t available);

            std::string mSlash; // string "/"?
            uint32_t mClassPathSize;
            std::vector<std::string> mClassPath;
            uint32_t mBootClassPathSize;
            std::vector<std::string> mBootClassPath;

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 13;
        };

        struct DisposeObjects : public JdwpReader {
            DisposeObjects(const uint8_t* bytes, uint32_t available);

            struct DispObj {
                ObjectId id;
                uint32_t refCount;
            };
            static std::string buildReq (
                    const std::vector<DispObj> &objs,int id = 0);
            const static uint8_t cmd = 14;
        };

        struct HoldEvents : public JdwpReader {
            HoldEvents(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 15;
        };

        struct ReleaseEvents : public JdwpReader {
            ReleaseEvents(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 16;
        };

        struct CapabilitiesNew : public Capabilities {
            CapabilitiesNew(const uint8_t* bytes, uint32_t available);

            uint8_t mCanRedefineClasses;
            uint8_t mCanAddMethod;
            uint8_t mCanUnrestrictedlyRedefineClasses;
            uint8_t mCanPopFrames;
            uint8_t mCanUseInstanceFilters;
            uint8_t mCanGetSourceDebugExtension;
            uint8_t mCanRequestVMDeathEvent;
            uint8_t mCanSetDefaultStratum;
            uint8_t mCanGetInstanceInfo;
            uint8_t mCanRequestMonitorEvents;
            uint8_t mCanGetMonitorFrameInfo;
            uint8_t mCanUseSourceNameFilters;
            uint8_t mCanGetConstantPool;
            uint8_t mCanForceEarlyReturn;
            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 17;
        };

        struct RedefineClasses : public JdwpReader {
            RedefineClasses(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 18;
        };

        struct SetDefaultStratum : public JdwpReader {
            SetDefaultStratum(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 19;
        };

        struct AllClassesWithGeneric : public JdwpReader {
            AllClassesWithGeneric(const uint8_t* bytes, uint32_t available);

            uint32_t mSize;
            std::vector<ClassInfo> mInfos;
            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 20;
        };

        struct InstanceCounts : public JdwpReader {
            InstanceCounts(const uint8_t* bytes, uint32_t available);

            uint32_t mCountSize;
            std::vector<uint64_t > mCounts;

            static std::string buildReq(
                    const std::vector<RefTypeId > &class_ids, int id = 0);
            const static uint8_t cmd = 21;
        };
    }
    namespace ReferenceType
    {
        uint8_t set_ = 2;

        struct Signature : public JdwpReader {
            Signature(const uint8_t* bytes, uint32_t available);

            std::string mSignature;
            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 1;
        };

        struct ClassLoader : public JdwpReader {
            ClassLoader(const uint8_t* bytes, uint32_t available);
            ObjectId mClassId;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 2;
        };

        struct Modifiers : public JdwpReader {
            Modifiers(const uint8_t* bytes, uint32_t available);
            uint32_t mFlags;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 3;
        };

        struct Fields : public JdwpReader {
            Fields(const uint8_t* bytes, uint32_t available);
            uint32_t mSize;
            std::vector<FieldInfo> mFields;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 4;
        };

        struct Methods : public JdwpReader {
            Methods(const uint8_t* bytes, uint32_t available);
            uint32_t mSize;
            std::vector<MethodInfo> mMethods;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 5;
        };

        struct GetValues : public JdwpReader {
            GetValues(const uint8_t* bytes, uint32_t available);
            bool isPrimateTag;

            JValue mValue;
            static std::string buildReq(RefTypeId refTypeId, int32_t field_count,
                                const std::vector<FieldId > &fieldids, int id = 0);
            const static uint8_t cmd = 6;
        };

        struct SourceFile : public JdwpReader {
            SourceFile(const uint8_t* bytes, uint32_t available);

            std::string mSourceFile;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 7;
        };

        struct NestedTypes : public JdwpReader {
            NestedTypes(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 8;
        };

        struct Status : public JdwpReader {
            Status(const uint8_t* bytes, uint32_t available);
            uint32_t mClassStatus;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 9;
        };

        struct Interfaces : public JdwpReader {
            Interfaces(const uint8_t* bytes, uint32_t available);
            uint32_t mCounts;
            std::vector<RefTypeId> mDirectInterfaces;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 10;
        };

        struct ClassObject : public JdwpReader {
            ClassObject(const uint8_t* bytes, uint32_t available);
            ObjectId mClassId;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 11;
        };

        struct SourceDebugExtension : public JdwpReader {
            SourceDebugExtension(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 12;
        };

        struct SignatureWithGeneric : public Signature {
            SignatureWithGeneric(const uint8_t* bytes, uint32_t available);
            std::string mSignatureGeneric;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 13;
        };

        struct FieldsWithGeneric : public JdwpReader {
            FieldsWithGeneric(const uint8_t* bytes, uint32_t available);
            uint32_t mSize;
            std::vector<FieldInfo> mFields;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 14;
        };

        struct MethodsWithGeneric : public JdwpReader {
            MethodsWithGeneric(const uint8_t* bytes, uint32_t available);
            uint32_t mSize;
            std::vector<MethodInfo> mMethods;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 15;
        };

        struct Instances : public JdwpReader {
            Instances(const uint8_t* bytes, uint32_t available);
            struct tagObj {
                JDWP::JdwpTag tag;
                ObjectId objectId;
            };
            uint32_t mSize;
            std::vector<tagObj> mObjTags;

            static std::string buildReq(RefTypeId refTypeId, int32_t maxcount,
                                        int id = 0);
            const static uint8_t cmd = 16;
        };

        struct ClassFileVersion : public JdwpReader {
            ClassFileVersion(const uint8_t* bytes, uint32_t available);


            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 17;
        };

        struct ConstantPool : public JdwpReader {
            ConstantPool(const uint8_t* bytes, uint32_t available);


            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 18;
        };
    }
    namespace ClassType
    {
        uint8_t set_ = 3;

        struct Superclass : public JdwpReader {
            Superclass(const uint8_t* bytes, uint32_t available);
            RefTypeId mSuperClassId;

            static std::string buildReq(RefTypeId refTypeId, int id = 0);
            const static uint8_t cmd = 1;
        };

        struct SetValues : public JdwpReader {
            SetValues(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(RefTypeId refTypeId,
                                        const std::vector<FieldInfo> &infos, int id = 0);
            const static uint8_t cmd = 2;
        };

        struct InvokeMethod : public JdwpReader {
            InvokeMethod(const uint8_t* bytes, uint32_t available);

            JValue mResult;
            JValue mObject;

            static std::string buildReq(RefTypeId class_id, ObjectId thread_id,
                          MethodId method_id,const std::vector<JValue> &argValues,
                          uint32_t options, int id = 0);
            const static uint8_t cmd = 3;
        };

        struct NewInstance : public JdwpReader {
            NewInstance(const uint8_t* bytes, uint32_t available);

            JValue mResult;
            JValue mObject;

            static std::string buildReq(RefTypeId class_id, ObjectId thread_id,
                         MethodId method_id,const std::vector<JValue> &argValues,
                         uint32_t options, int id = 0);
            const static uint8_t cmd = 4;
        };
    }

    namespace ArrayType
    {
        uint8_t set_ = 4;

        struct NewInstance : public JdwpReader {
            NewInstance(const uint8_t* bytes, uint32_t available);

            JValue mObject;

            static std::string buildReq(RefTypeId arrayTypeId, uint32_t length,
                                        int id = 0);
            const static uint8_t cmd = 1;
        };
    }

    namespace InterfaceType
    {
        uint8_t set_ = 5;
    }

    namespace Method
    {
        uint8_t set_ = 6;

        struct LineTable : public JdwpReader {
            struct LineLabel {
                uint64_t address;
                uint32_t line_number;
            };
        public:
            LineTable(const uint8_t* bytes, uint32_t available);

            uint64_t mStart;
            uint64_t mEnd;

            size_t mNumLines;
            std::vector<LineLabel> mItems;

            static std::string buildReq(RefTypeId refTypeId, MethodId method_id,
                                        int id = 0);
            const static uint8_t cmd = 1;
        };

        struct VariableTable : public JdwpReader {
            struct VarLabel {
                uint64_t startAddress;
                std::string name;
                std::string descriptor;
                std::string signature;
                uint32_t size;
                uint32_t slot;
            };
        public:
            VariableTable(const uint8_t* bytes, uint32_t available);

            uint32_t mNumArgRegisters;
            size_t mVariableCount;
            std::vector<VarLabel> mItems;

            static std::string buildReq(RefTypeId class_id, MethodId method_id,
                                        int id = 0);
            const static uint8_t cmd = 2;
        };

        struct Bytecodes : public JdwpReader {
            Bytecodes(const uint8_t* bytes, uint32_t available);

            uint32_t mByteCodesSize;
            std::vector<uint8_t > mByteCodes;
            static std::string buildReq(RefTypeId class_id, MethodId method_id,
                                        int id = 0);
            const static uint8_t cmd = 3;
        };

        struct IsObsolete : public JdwpReader {
            IsObsolete(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(RefTypeId class_id, MethodId method_id,
                                        int id = 0);
            const static uint8_t cmd = 4;
        };

        struct VariableTableWithGeneric : public JdwpReader {
            struct VarLabel {
                uint64_t startAddress;
                std::string name;
                std::string descriptor;
                std::string signature;
                uint32_t size;
                uint32_t slot;
            };
        public:
            VariableTableWithGeneric(const uint8_t* bytes, uint32_t available);

            uint32_t mNumArgRegisters;
            size_t mVariableCount;
            std::vector<VarLabel> mItems;

            static std::string buildReq(RefTypeId class_id, MethodId method_id,
                                        int id = 0);
            const static uint8_t cmd = 5;
        };
    }

    namespace Field
    {
        uint8_t set_ = 8;
    }

    namespace ObjectReference
    {
        uint8_t set_ = 9;

        struct ReferenceType : public JdwpReader {
            ReferenceType(const uint8_t* bytes, uint32_t available);

            JdwpTypeTag tag;
            RefTypeId mTypeId;

            static std::string buildReq(ObjectId object_id, int id = 0);
            const static uint8_t cmd = 1;
        };

        struct GetValues : public JdwpReader {
            GetValues(const uint8_t* bytes, uint32_t available);
            JValue mValue;

            static std::string buildReq(ObjectId object_id, const std::vector<FieldId > &fields,
                                        int id = 0);
            const static uint8_t cmd = 2;
        };

        struct SetValues : public JdwpReader {
            SetValues(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(ObjectId object_id, const std::vector<FieldInfo> &fields,
                                        int id = 0);
            const static uint8_t cmd = 3;
        };

        struct UNUSED : public JdwpReader {
            UNUSED(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(int id = 0);
            const static uint8_t cmd = 4;
        };

        struct MonitorInfo : public JdwpReader {
            MonitorInfo(const uint8_t* bytes, uint32_t available);

            ObjectId mMonitorInfo;
            uint32_t mEntryCount;
            uint32_t mWaiterSize;
            std::vector<ObjectId> mWaiters;

            static std::string buildReq(ObjectId object_id, int id = 0);
            const static uint8_t cmd = 5;
        };

        struct InvokeMethod : public JdwpReader {
            InvokeMethod(const uint8_t* bytes, uint32_t available);

            JValue mResult;
            JValue mObject;

            static std::string buildReq(ObjectId object_id, ObjectId thread_id,
                                        RefTypeId class_id, MethodId method_id,
                                        const std::vector<JValue> &argValues,uint32_t options,
                                        int id = 0);


            const static uint8_t cmd = 6;
        };

        struct DisableCollection : public JdwpReader {
            DisableCollection(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(ObjectId object_id,int id = 0);

            const static uint8_t cmd = 7;
        };

        struct EnableCollection : public JdwpReader {
            EnableCollection(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(ObjectId object_id,int id = 0);

            const static uint8_t cmd = 8;
        };

        struct IsCollected : public JdwpReader {
            IsCollected(const uint8_t* bytes, uint32_t available);

            bool mIsCollected;
            static std::string buildReq(ObjectId object_id,int id = 0);

            const static uint8_t cmd = 9;
        };

        struct ReferringObjects : public JdwpReader {
            ReferringObjects(const uint8_t* bytes, uint32_t available);

            int mSize;
            std::vector<JValue> mObjs;
            static std::string buildReq(ObjectId object_id, int32_t max_count, int id = 0);

            const static uint8_t cmd = 10;
        };
    }

    namespace StringReference
    {
        uint8_t set_ = 10;

        struct Value : public JdwpReader {
            Value(const uint8_t* bytes, uint32_t available);

            std::string mStr;
            static std::string buildReq(ObjectId stringObject, int id = 0);

            const static uint8_t cmd = 1;
        };
    }

    namespace ThreadReference
    {
        uint8_t set_ = 11;

        struct Name : public JdwpReader {
            Name(const uint8_t* bytes, uint32_t available);

            std::string mName;
            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 1;
        };

        struct Suspend : public JdwpReader {
            Suspend(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 2;
        };

        struct Resume : public JdwpReader {
            Resume(const uint8_t* bytes, uint32_t available);

            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 3;
        };

        struct Status : public JdwpReader {
            Status(const uint8_t* bytes, uint32_t available);
            uint32_t mThreadStatus;
            uint32_t mSuspendStatus;
            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 4;
        };

        struct ThreadGroup : public JdwpReader {
            ThreadGroup(const uint8_t* bytes, uint32_t available);
            ObjectId mThreadGroupId;
            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 5;
        };

        struct Frames : public JdwpReader {
            struct Frame {
                ObjectId frame_id;
                JdwpLocation location;
            };
        public:
            Frames(const uint8_t* bytes, uint32_t available);
            int mFrameCount;
            std::vector<Frame> mFrames;

            static std::string buildReq(ObjectId thread_id, uint32_t start_frame,
                                        uint32_t length, int id = 0);

            const static uint8_t cmd = 6;
        };

        struct FrameCount : public JdwpReader {
            FrameCount(const uint8_t* bytes, uint32_t available);
            int mFrameCount;

            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 7;
        };

        struct OwnedMonitors : public JdwpReader {
            OwnedMonitors(const uint8_t* bytes, uint32_t available);
            int mMonitorSize;
            std::vector<JValue> mMonitors;
            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 8;
        };

        struct CurrentContendedMonitor : public JdwpReader {
            CurrentContendedMonitor(const uint8_t* bytes, uint32_t available);
            JValue mContendedMonitor;
            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 9;
        };

        struct Stop : public JdwpReader {
            Stop(const uint8_t* bytes, uint32_t available);
            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 10;
        };

        struct Interrupt : public JdwpReader {
            Interrupt(const uint8_t* bytes, uint32_t available);
            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 11;
        };

        struct SuspendCount : public JdwpReader {
            SuspendCount(const uint8_t* bytes, uint32_t available);
            uint32_t mSuspendCount;

            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 12;
        };

        struct OwnedMonitorsStackDepthInfo : public JdwpReader {
            struct Monitor{
                JValue monitor;
                uint32_t depth;
            };
        public:
            OwnedMonitorsStackDepthInfo(const uint8_t* bytes, uint32_t available);
            int mMonitorSize;
            std::vector<Monitor> mMonitors;
            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 13;
        };

        struct ForceEarlyReturn : public JdwpReader {
            ForceEarlyReturn(const uint8_t* bytes, uint32_t available);
            static std::string buildReq(ObjectId thread_id, int id = 0);

            const static uint8_t cmd = 14;
        };
    }

    namespace ThreadGroupReference
    {
        uint8_t set_ = 12;
        struct ForceEarlyReturn : public JdwpReader {
            ForceEarlyReturn(const uint8_t* bytes, uint32_t available);

            std::string mThreadGroupName;

            static std::string buildReq(ObjectId thread_group_id, int id = 0);

            const static uint8_t cmd = 1;
        };

        struct Parent : public JdwpReader {
            Parent(const uint8_t* bytes, uint32_t available);

            ObjectId mThreadParentgroup;

            static std::string buildReq(ObjectId thread_group_id, int id = 0);

            const static uint8_t cmd = 2;
        };

        struct Children : public JdwpReader {
            Children(const uint8_t* bytes, uint32_t available);

            int mThreadIdSize;
            std::vector<ObjectId> mThreadIds;
            int mChildThreadGroupIdSize;
            std::vector<ObjectId> mChildThreadGroupsIds;

            static std::string buildReq(ObjectId thread_group_id, int id = 0);

            const static uint8_t cmd = 3;
        };
    }


    bool Write1(std::string &s, uint8_t v);
    bool Write2(std::string &s, uint16_t v);
    bool Write4(std::string &s, uint32_t v);
    bool Write8(std::string &s, uint64_t v);
    bool WriteValue(std::string &s, uint64_t v, size_t width);
    bool WriteString(std::string &s, const std::string &l);
    bool WriteFieldId(std::string &s, uint32_t v);
    bool WriteMethodId(std::string &s, uint32_t v);
    bool WriteObjectId(std::string &s, uint64_t v);
    bool WriteRefTypeId(std::string &s, uint64_t v);
    bool WriteFrameId(std::string &s, uint64_t v);
    bool WriteThreadId(std::string &s, uint64_t v);
    std::string BuildReq(int id, int cmdset, int cmd, const std::string &extra);
}


#endif //PROJECT_JDWPHANDLER_H

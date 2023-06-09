// automatically generated by the FlatBuffers compiler, do not modify

package jvm_toolbox_flatbuffers.inspector;

import com.google.flatbuffers.BaseVector;
import com.google.flatbuffers.BooleanVector;
import com.google.flatbuffers.ByteVector;
import com.google.flatbuffers.Constants;
import com.google.flatbuffers.DoubleVector;
import com.google.flatbuffers.FlatBufferBuilder;
import com.google.flatbuffers.FloatVector;
import com.google.flatbuffers.IntVector;
import com.google.flatbuffers.LongVector;
import com.google.flatbuffers.ShortVector;
import com.google.flatbuffers.StringVector;
import com.google.flatbuffers.Struct;
import com.google.flatbuffers.Table;
import com.google.flatbuffers.UnionVector;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

@SuppressWarnings("unused")
public final class OnFieldAccess extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_23_5_9(); }
  public static OnFieldAccess getRootAsOnFieldAccess(ByteBuffer _bb) { return getRootAsOnFieldAccess(_bb, new OnFieldAccess()); }
  public static OnFieldAccess getRootAsOnFieldAccess(ByteBuffer _bb, OnFieldAccess obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public OnFieldAccess __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public jvm_toolbox_flatbuffers.inspector.JavaThread thread() { return thread(new jvm_toolbox_flatbuffers.inspector.JavaThread()); }
  public jvm_toolbox_flatbuffers.inspector.JavaThread thread(jvm_toolbox_flatbuffers.inspector.JavaThread obj) { int o = __offset(4); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }
  public jvm_toolbox_flatbuffers.inspector.JavaMethod method() { return method(new jvm_toolbox_flatbuffers.inspector.JavaMethod()); }
  public jvm_toolbox_flatbuffers.inspector.JavaMethod method(jvm_toolbox_flatbuffers.inspector.JavaMethod obj) { int o = __offset(6); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }
  public long location() { int o = __offset(8); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public jvm_toolbox_flatbuffers.inspector.JavaClass class_() { return class_(new jvm_toolbox_flatbuffers.inspector.JavaClass()); }
  public jvm_toolbox_flatbuffers.inspector.JavaClass class_(jvm_toolbox_flatbuffers.inspector.JavaClass obj) { int o = __offset(10); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }
  public jvm_toolbox_flatbuffers.inspector.JavaObject object() { return object(new jvm_toolbox_flatbuffers.inspector.JavaObject()); }
  public jvm_toolbox_flatbuffers.inspector.JavaObject object(jvm_toolbox_flatbuffers.inspector.JavaObject obj) { int o = __offset(12); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }
  public jvm_toolbox_flatbuffers.inspector.JavaField field() { return field(new jvm_toolbox_flatbuffers.inspector.JavaField()); }
  public jvm_toolbox_flatbuffers.inspector.JavaField field(jvm_toolbox_flatbuffers.inspector.JavaField obj) { int o = __offset(14); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }

  public static int createOnFieldAccess(FlatBufferBuilder builder,
      int threadOffset,
      int methodOffset,
      long location,
      int class_Offset,
      int objectOffset,
      int fieldOffset) {
    builder.startTable(6);
    OnFieldAccess.addLocation(builder, location);
    OnFieldAccess.addField(builder, fieldOffset);
    OnFieldAccess.addObject(builder, objectOffset);
    OnFieldAccess.addClass(builder, class_Offset);
    OnFieldAccess.addMethod(builder, methodOffset);
    OnFieldAccess.addThread(builder, threadOffset);
    return OnFieldAccess.endOnFieldAccess(builder);
  }

  public static void startOnFieldAccess(FlatBufferBuilder builder) { builder.startTable(6); }
  public static void addThread(FlatBufferBuilder builder, int threadOffset) { builder.addOffset(0, threadOffset, 0); }
  public static void addMethod(FlatBufferBuilder builder, int methodOffset) { builder.addOffset(1, methodOffset, 0); }
  public static void addLocation(FlatBufferBuilder builder, long location) { builder.addLong(2, location, 0L); }
  public static void addClass(FlatBufferBuilder builder, int class_Offset) { builder.addOffset(3, class_Offset, 0); }
  public static void addObject(FlatBufferBuilder builder, int objectOffset) { builder.addOffset(4, objectOffset, 0); }
  public static void addField(FlatBufferBuilder builder, int fieldOffset) { builder.addOffset(5, fieldOffset, 0); }
  public static int endOnFieldAccess(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public OnFieldAccess get(int j) { return get(new OnFieldAccess(), j); }
    public OnFieldAccess get(OnFieldAccess obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}


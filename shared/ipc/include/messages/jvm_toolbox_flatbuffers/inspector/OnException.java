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
public final class OnException extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_23_5_9(); }
  public static OnException getRootAsOnException(ByteBuffer _bb) { return getRootAsOnException(_bb, new OnException()); }
  public static OnException getRootAsOnException(ByteBuffer _bb, OnException obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public OnException __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public jvm_toolbox_flatbuffers.inspector.JavaThread thread() { return thread(new jvm_toolbox_flatbuffers.inspector.JavaThread()); }
  public jvm_toolbox_flatbuffers.inspector.JavaThread thread(jvm_toolbox_flatbuffers.inspector.JavaThread obj) { int o = __offset(4); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }
  public jvm_toolbox_flatbuffers.inspector.JavaMethod method() { return method(new jvm_toolbox_flatbuffers.inspector.JavaMethod()); }
  public jvm_toolbox_flatbuffers.inspector.JavaMethod method(jvm_toolbox_flatbuffers.inspector.JavaMethod obj) { int o = __offset(6); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }
  public long location() { int o = __offset(8); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public jvm_toolbox_flatbuffers.inspector.JavaObject exception() { return exception(new jvm_toolbox_flatbuffers.inspector.JavaObject()); }
  public jvm_toolbox_flatbuffers.inspector.JavaObject exception(jvm_toolbox_flatbuffers.inspector.JavaObject obj) { int o = __offset(10); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }
  public jvm_toolbox_flatbuffers.inspector.JavaMethod catchMethod() { return catchMethod(new jvm_toolbox_flatbuffers.inspector.JavaMethod()); }
  public jvm_toolbox_flatbuffers.inspector.JavaMethod catchMethod(jvm_toolbox_flatbuffers.inspector.JavaMethod obj) { int o = __offset(12); return o != 0 ? obj.__assign(__indirect(o + bb_pos), bb) : null; }
  public long catchLocation() { int o = __offset(14); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }

  public static int createOnException(FlatBufferBuilder builder,
      int threadOffset,
      int methodOffset,
      long location,
      int exceptionOffset,
      int catchMethodOffset,
      long catchLocation) {
    builder.startTable(6);
    OnException.addCatchLocation(builder, catchLocation);
    OnException.addLocation(builder, location);
    OnException.addCatchMethod(builder, catchMethodOffset);
    OnException.addException(builder, exceptionOffset);
    OnException.addMethod(builder, methodOffset);
    OnException.addThread(builder, threadOffset);
    return OnException.endOnException(builder);
  }

  public static void startOnException(FlatBufferBuilder builder) { builder.startTable(6); }
  public static void addThread(FlatBufferBuilder builder, int threadOffset) { builder.addOffset(0, threadOffset, 0); }
  public static void addMethod(FlatBufferBuilder builder, int methodOffset) { builder.addOffset(1, methodOffset, 0); }
  public static void addLocation(FlatBufferBuilder builder, long location) { builder.addLong(2, location, 0L); }
  public static void addException(FlatBufferBuilder builder, int exceptionOffset) { builder.addOffset(3, exceptionOffset, 0); }
  public static void addCatchMethod(FlatBufferBuilder builder, int catchMethodOffset) { builder.addOffset(4, catchMethodOffset, 0); }
  public static void addCatchLocation(FlatBufferBuilder builder, long catchLocation) { builder.addLong(5, catchLocation, 0L); }
  public static int endOnException(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public OnException get(int j) { return get(new OnException(), j); }
    public OnException get(OnException obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}


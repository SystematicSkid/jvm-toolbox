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
public final class JavaMethod extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_23_5_9(); }
  public static JavaMethod getRootAsJavaMethod(ByteBuffer _bb) { return getRootAsJavaMethod(_bb, new JavaMethod()); }
  public static JavaMethod getRootAsJavaMethod(ByteBuffer _bb, JavaMethod obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public JavaMethod __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public String name() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer nameAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public ByteBuffer nameInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 4, 1); }
  public String signature() { int o = __offset(6); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer signatureAsByteBuffer() { return __vector_as_bytebuffer(6, 1); }
  public ByteBuffer signatureInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 6, 1); }
  public long address() { int o = __offset(8); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }

  public static int createJavaMethod(FlatBufferBuilder builder,
      int nameOffset,
      int signatureOffset,
      long address) {
    builder.startTable(3);
    JavaMethod.addAddress(builder, address);
    JavaMethod.addSignature(builder, signatureOffset);
    JavaMethod.addName(builder, nameOffset);
    return JavaMethod.endJavaMethod(builder);
  }

  public static void startJavaMethod(FlatBufferBuilder builder) { builder.startTable(3); }
  public static void addName(FlatBufferBuilder builder, int nameOffset) { builder.addOffset(0, nameOffset, 0); }
  public static void addSignature(FlatBufferBuilder builder, int signatureOffset) { builder.addOffset(1, signatureOffset, 0); }
  public static void addAddress(FlatBufferBuilder builder, long address) { builder.addLong(2, address, 0L); }
  public static int endJavaMethod(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public JavaMethod get(int j) { return get(new JavaMethod(), j); }
    public JavaMethod get(JavaMethod obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}


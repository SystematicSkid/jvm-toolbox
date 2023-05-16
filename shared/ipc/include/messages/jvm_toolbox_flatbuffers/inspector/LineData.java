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
public final class LineData extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_23_5_9(); }
  public static LineData getRootAsLineData(ByteBuffer _bb) { return getRootAsLineData(_bb, new LineData()); }
  public static LineData getRootAsLineData(ByteBuffer _bb, LineData obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public LineData __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public int lineNumber() { int o = __offset(4); return o != 0 ? bb.getInt(o + bb_pos) : 0; }
  public int bytecodeIndex() { int o = __offset(6); return o != 0 ? bb.getInt(o + bb_pos) : 0; }

  public static int createLineData(FlatBufferBuilder builder,
      int lineNumber,
      int bytecodeIndex) {
    builder.startTable(2);
    LineData.addBytecodeIndex(builder, bytecodeIndex);
    LineData.addLineNumber(builder, lineNumber);
    return LineData.endLineData(builder);
  }

  public static void startLineData(FlatBufferBuilder builder) { builder.startTable(2); }
  public static void addLineNumber(FlatBufferBuilder builder, int lineNumber) { builder.addInt(0, lineNumber, 0); }
  public static void addBytecodeIndex(FlatBufferBuilder builder, int bytecodeIndex) { builder.addInt(1, bytecodeIndex, 0); }
  public static int endLineData(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public LineData get(int j) { return get(new LineData(), j); }
    public LineData get(LineData obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}


module mux16 (/*AUTOARG*/
    // Outputs
    out,
    // Inputs
    sel,
    in0,
    in1,
    in2,
    in3,
    in4,
    in5,
    in6,
    in7,
    in8,
    in9,
    in10,
    in11,
    in12,
    in13,
    in14,
    in15
    ) ;
    parameter DATA_WIDTH = 32;

    output reg [DATA_WIDTH-1:0] out;
    input wire [3:0] 	       sel;

    input wire [DATA_WIDTH-1:0] in0;
    input wire [DATA_WIDTH-1:0] in1;
    input wire [DATA_WIDTH-1:0] in2;
    input wire [DATA_WIDTH-1:0] in3;
    input wire [DATA_WIDTH-1:0] in4;
    input wire [DATA_WIDTH-1:0] in5;
    input wire [DATA_WIDTH-1:0] in6;
    input wire [DATA_WIDTH-1:0] in7;
    input wire [DATA_WIDTH-1:0] in8;
    input wire [DATA_WIDTH-1:0] in9;
    input wire [DATA_WIDTH-1:0] in10;
    input wire [DATA_WIDTH-1:0] in11;
    input wire [DATA_WIDTH-1:0] in12;
    input wire [DATA_WIDTH-1:0] in13;
    input wire [DATA_WIDTH-1:0] in14;
    input wire [DATA_WIDTH-1:0] in15;
    wire [DATA_WIDTH-1:0] 	       out0;
    wire [DATA_WIDTH-1:0] 	       out1;


    mux8 #(.DATA_WIDTH(DATA_WIDTH))
    u_mux8_0( .out(out0),
	      .sel(sel[2:0]),
	      .in0(in0),
	      .in1(in1),
	      .in2(in2),
	      .in3(in3),
	      .in4(in4),
	      .in5(in5),
	      .in6(in6),
	      .in7(in7));

    mux8 #(.DATA_WIDTH(DATA_WIDTH))
    u_mux8_1( .out(out1),
	      .sel(sel[2:0]),
	      .in0(in8),
	      .in1(in9),
	      .in2(in10),
	      .in3(in11),
	      .in4(in12),
	      .in5(in13),
	      .in6(in14),
	      .in7(in15)
	      );


    always @ (*) begin
	case (sel[3-:1])
	    1'b0: begin
		out <= out0;
	    end
	    1'b1: begin
		out <= out1;
	    end // case: 1'b1
	endcase
    end
endmodule // mux

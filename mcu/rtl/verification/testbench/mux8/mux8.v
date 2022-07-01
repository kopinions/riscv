module mux8 (/*AUTOARG*/
	     // Outputs
	     out,
	     // Inputs
	     sel, in0, in1, in2, in3, in4, in5, in6, in7
	     ) ;
   parameter DATA_WIDTH = 32;

   output reg [DATA_WIDTH-1:0] out;
   
   input wire [2:0] 	       sel;
   input wire [DATA_WIDTH-1:0] in0;
   input wire [DATA_WIDTH-1:0] in1;
   input wire [DATA_WIDTH-1:0] in2;
   input wire [DATA_WIDTH-1:0] in3;
   input wire [DATA_WIDTH-1:0] in4;
   input wire [DATA_WIDTH-1:0] in5;
   input wire [DATA_WIDTH-1:0] in6;
   input wire [DATA_WIDTH-1:0] in7;
   
   wire [DATA_WIDTH-1:0]       out0;
   wire [DATA_WIDTH-1:0]       out1;
   

   mux4 #(.DATA_WIDTH(DATA_WIDTH))
   u_mux4_0( .out(out0), 
	     .sel(sel[1:0]), 
	     .in0(in0), 	
	     .in1(in1),
	     .in2(in2),
	     .in3(in3)
	     );
   
   
   mux4 #(.DATA_WIDTH(DATA_WIDTH))
   u_mux4_1( .out(out1),
	     .sel(sel[1:0]),
	     .in0(in4),
	     .in1(in5),
	     .in2(in6),
	     .in3(in7)
	     );


   always @ (*) begin
      case (sel[2-:1]) 
	1'b0: begin
	   out = out0;
	end
	1'b1: begin
	   out = out1;
	end
      endcase
   end
endmodule // mux8

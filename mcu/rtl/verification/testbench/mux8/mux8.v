module mux8 (/*AUTOARG*/
   // Outputs
   out,
   // Inputs
   sel, in0, in1, in2, in3, in4, in5, in6, in7
   ) ;
   parameter DATA_WIDTH = 32;
   
   input wire [2:0] sel;
   input wire [DATA_WIDTH-1:0] in0;
   input wire [DATA_WIDTH-1:0] in1;
   input wire [DATA_WIDTH-1:0] in2;
   input wire [DATA_WIDTH-1:0] in3;
   input wire [DATA_WIDTH-1:0] in4;
   input wire [DATA_WIDTH-1:0] in5;
   input wire [DATA_WIDTH-1:0] in6;
   input wire [DATA_WIDTH-1:0] in7;
   output reg [DATA_WIDTH-1:0] out;

   always @ (*) begin
      case (sel) 
	3'b000: begin
	   out = in0;
	end
	3'b001: begin
	   out = in1;
	end
	3'b010: begin
	   out = in2;
	end
	3'b011: begin
	   out = in3;
	end
	3'b100: begin
	   out = in4;
	end
	3'b101: begin
	   out = in5;
	end
	3'b110: begin
	   out = in6;
	end
	3'b111: begin
	   out = in7;
	end
      endcase
   end
endmodule // mux

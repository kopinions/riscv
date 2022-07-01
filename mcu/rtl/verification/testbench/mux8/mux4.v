module mux4 (/*AUTOARG*/
	     // Outputs
	     out,
	     // Inputs
	     sel, in0, in1, in2, in3
	     ) ;
   parameter DATA_WIDTH = 32;
   
   output reg [DATA_WIDTH-1:0] out;
   input wire [1:0] 	       sel;
   
   input wire [DATA_WIDTH-1:0] in0;
   input wire [DATA_WIDTH-1:0] in1;
   input wire [DATA_WIDTH-1:0] in2;
   input wire [DATA_WIDTH-1:0] in3;
   

   always @ (*) begin
      case (sel) 
	2'b00: begin
	   out = in0;
	end
	2'b01: begin
	   out = in1;
	end
	2'b10: begin
	   out = in2;
	end
	2'b11: begin
	   out = in3;
	end
	default: begin
	   out= {DATA_WIDTH{1'b0}};
	end
      endcase
   end
endmodule // mux4

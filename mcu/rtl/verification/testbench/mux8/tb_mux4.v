module tb_mux4 (/*AUTOARG*/) ;
   reg [63:0] in0;
   reg [63:0] in1;
   reg [63:0] in2;
   reg [63:0] in3;
   
   wire [63:0] out;
   reg [1:0]   sel;

   mux4 #(.DATA_WIDTH(64))
   u_mux4(/*AUDOINST*/
	  .in0(in0),
	  .in1(in1),
	  .in2(in2),
	  .in3(in3),
	  .sel(sel),
	  .out(out)
	  );

   integer     i;
   
   initial begin
      $monitor("%h", out);
      
      in0 = 64'h0;
      in1 = 64'h1;
      in2 = 64'h2;
      in3 = 64'h3;

      for (i=0;i<4;i++) begin
	 #1 sel=i;
      end
   end
   
   
endmodule // tb_mux8

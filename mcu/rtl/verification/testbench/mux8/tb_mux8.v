module tb_mux8 (/*AUTOARG*/) ;
   reg [63:0] in0;
   reg [63:0] in1;
   reg [63:0] in2;
   reg [63:0] in3;
   reg [63:0] in4;
   reg [63:0] in5;
   reg [63:0] in6;
   reg [63:0] in7;
   wire [63:0] out;
   reg [2:0]   sel;

   mux8 #(.DATA_WIDTH(64))
   u_mux8(/*AUDOINST*/
	  .in0(in0),
	  .in1(in1),
	  .in2(in2),
	  .in3(in3),
	  .in4(in4),
	  .in5(in5),
	  .in6(in6),
	  .in7(in7),
	  .sel(sel),
	  .out(out)
	  );

   integer     i;
   
   initial begin
      sel = 0;
      
      $monitor("sel:%h out:%h", sel, out);
      
      in0 = 64'h0;
      in1 = 64'h1;
      in2 = 64'h2;
      in3 = 64'h3;
      in4 = 64'h4;
      in5 = 64'h5;
      in6 = 64'h6;
      in7 = 64'h7;

      for (i=0;i<8;i++) begin
	 #1 sel=i;
      end
   end
   
   
endmodule // tb_mux8

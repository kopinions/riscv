module tb_mux16 (/*AUTOARG*/) ;
   reg [63:0] in0;
   reg [63:0] in1;
   reg [63:0] in2;
   reg [63:0] in3;
   reg [63:0] in4;
   reg [63:0] in5;
   reg [63:0] in6;
   reg [63:0] in7;
   reg [63:0] in8;
   reg [63:0] in9;
   reg [63:0] in10;
   reg [63:0] in11;
   reg [63:0] in12;
   reg [63:0] in13;
   reg [63:0] in14;
   reg [63:0] in15;
   wire [63:0] out;
   reg [3:0]   sel;

   mux16 #(.DATA_WIDTH(64))
   u_mux16(/*AUDOINST*/
	   .out(out),
	   .sel(sel),
	   .in0(in0),
	   .in1(in1),
	   .in2(in2),
	   .in3(in3),
	   .in4(in4),
	   .in5(in5),
	   .in6(in6),
	   .in7(in7),
	   .in8(in8),
	   .in9(in9),
	   .in10(in10),
	   .in11(in11),
	   .in12(in12),
	   .in13(in13),
	   .in14(in14),
	   .in15(in15)
	   );

   integer     i;
   
   initial begin
      sel = 4'b0;
      
      in0 = 64'h0;
      in1 = 64'h1;
      in2 = 64'h2;
      in3 = 64'h3;
      in4 = 64'h4;
      in5 = 64'h5;
      in6 = 64'h6;
      in7 = 64'h7;
      in8 = 64'h8;
      in9 = 64'h9;
      in10 = 64'h10;
      in11 = 64'h11;
      in12 = 64'h12;
      in13 = 64'h13;
      in14 = 64'h14;
      in15 = 64'h15;
      
      $monitor("sel: %h out: %h", sel, out);

      for (i=0;i<16;i++) begin
	 #1 sel=i;
      end
   end
   
   
endmodule // tb_mux16

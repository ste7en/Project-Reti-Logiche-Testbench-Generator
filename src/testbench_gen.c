#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char* argv[]) {
  char fpname[16];
  strcpy(fpname, "testbench_0.vhd");
  srand(time(NULL));

  for(int a = 48; a < 53; a++){

    srand(rand());

    printf("\nIteration %d", a);
    FILE *fp = NULL;
    int arr[19];
    int minDist = 255+255;
    int outMask;
    int inMask = 0;
    int inMaskBool[8];

    fpname[10] = a;

    fp = fopen(fpname, "w+");

    if (fp == NULL) { printf("Couldn't open the file for writing.\n"); return -1; }

    for(int i = 1; i <= 18; i++) {
      arr[i] = (rand() % 256);
      srand(rand());
    }

    for(int i = 0; i < 8; i++) {
      inMaskBool[i] = rand() % 2;
      if (inMaskBool[i]) {
        inMask += pow(2.0, (double)i);
      }
    }

    int pointX = arr[17];
    int pointY = arr[18];

    for(int i = 0; i < 8; i++) {
      if (!inMaskBool[i]) {continue;}
      int tempX = arr[i*2+1];
      int tempY = arr[i*2+2];
      int distance = abs(tempX-pointX)+abs(tempY-pointY);

      if (distance <= minDist) {

        if (distance < minDist) {
          minDist = distance;
          outMask = 0;
        }
        outMask += (int)pow(2.0, (double)i);
      }

    }

    fprintf(fp, "library ieee;\nuse ieee.std_logic_1164.all;\nuse ieee.numeric_std.all;\nuse ieee.std_logic_unsigned.all;entity project_tb is\nend project_tb;\n\narchitecture projecttb of project_tb is\nconstant c_CLOCK_PERIOD		: time := 100 ns;\nsignal   tb_done		: std_logic;\nsignal   mem_address		: std_logic_vector (15 downto 0) := (others => '0');\nsignal   tb_rst	                : std_logic := '0';\nsignal   tb_start		: std_logic := '0';\nsignal   tb_clk		        : std_logic := '0';\nsignal   mem_o_data,mem_i_data	: std_logic_vector (7 downto 0);\nsignal   enable_wire  		: std_logic;\nsignal   mem_we		        : std_logic;\n\ntype ram_type is array (65535 downto 0) of std_logic_vector(7 downto 0);\n\n-- come da esempio su specifica\nsignal RAM: ram_type := (");

    fprintf(fp, "%d => std_logic_vector(to_unsigned( %d , 8)),\n\t\t\t ", 0, inMask);

    for (int i = 1; i < 19; i++) {
      fprintf(fp, "%d => std_logic_vector(to_unsigned( %d , 8)),\n\t\t\t ", i, arr[i]);
    }

    fprintf(fp, "others => (others =>'0'));\n\ncomponent project_reti_logiche is\nport (\n\ti_clk\t\t : in\tstd_logic;\n\ti_start\t\t : in\tstd_logic;\n\ti_rst\t\t : in\tstd_logic;\n\ti_data\t\t : in\tstd_logic_vector(7 downto 0);\n\to_address\t : out std_logic_vector(15 downto 0);\n\to_done\t\t : out std_logic;\n\to_en\t\t : out std_logic;\n\to_we\t\t : out std_logic;\n\to_data\t\t : out std_logic_vector (7 downto 0)\n\t);\nend component project_reti_logiche;\n\n\nbegin\nUUT: project_reti_logiche\nport map (\n\t\ti_clk\t	=> tb_clk,\n\t\ti_start\t\t=> tb_start,\n\t\ti_rst\t	=> tb_rst,\n\t\ti_data\t	=> mem_o_data,\n\t\to_address\t	=> mem_address,\n\t\to_done\t	=> tb_done,\n\t\to_en\t 	=> enable_wire,\n\t\to_we 		=> mem_we,\n\t\to_data\t	=> mem_i_data\n\t\t);\n\np_CLK_GEN : process is\nbegin\n\twait for c_CLOCK_PERIOD/2;\n\ttb_clk <= not tb_clk;\nend process p_CLK_GEN;\n\n\nMEM : process(tb_clk)\nbegin\n\tif tb_clk'event and tb_clk = '1' then\n\tif enable_wire = '1' then\n\t\tif mem_we = '1' then\n\t\t\tRAM(conv_integer(mem_address))\t<= mem_i_data;\n\t\t\tmem_o_data <= mem_i_data after 2 ns;\n\t\telse\n\t\t\tmem_o_data <= RAM(conv_integer(mem_address)) after 2 ns;\n\t\tend if;\n\tend if;\n\tend if;\nend process;\n\n\ntest : process is\nbegin\n\twait for 100 ns;\n\twait for c_CLOCK_PERIOD;\n\ttb_rst <= '1';\n\twait for c_CLOCK_PERIOD;\n\ttb_rst <= '0';\n\twait for c_CLOCK_PERIOD;\n\ttb_start <= '1';\n\twait for c_CLOCK_PERIOD;\n\twait until tb_done = '1';\n\twait for c_CLOCK_PERIOD;\n\ttb_start <= '0';\n\twait until tb_done = '0';\n\n");

    fprintf(fp, "-- Maschera di output \nassert RAM(19) = std_logic_vector(to_unsigned( %d , 8)) report \"TEST FALLITO\" severity failure;\n\nassert false report \"Simulation Ended!, TEST PASSATO\" severity failure;\nend process test;\n\nend projecttb;\n", outMask);

    fclose(fp);
  }
  return 0;
}

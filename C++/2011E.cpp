# include<iostream>
# include<fstream>
using namespace std;

# define SYS_CLK 10000000
# define DATA_RATE 100000
# define PLL_CLK 2*DATA_RATE
# define NUM_DATA 100000


ifstream f;

int main(){

    // * -------- Read Data In From File -------------------
    f.open("C:/Users/A_suozhang/Code/EE/Verilog/2011E/out.txt");
    char data_in[NUM_DATA] = {0};
    int i = 0;
    if (f.is_open()){
        while ((!f.eof()) && (i<NUM_DATA)){
            f >> data_in[i];
            i++;
            // cout << i << endl;
        }
    }
    f.close();

    // * ------- Creating Data Edge -----------
    char data_in_edge[NUM_DATA] = {0};
    for (int i=1; i< NUM_DATA;i++){
        if (data_in[i-1] == 0 && data_in[i] == 1){
            data_in_edge[i] = 1;
        }
        else{
            data_in_edge[i] = 0;
        }
    }

    // * ------- Create PLL CLK EDGE---------------------
    char pll_clk_edge[NUM_DATA] = {0};
    for (int i = 0; i< NUM_DATA; i++){
        if (i%(SYS_CLK/(PLL_CLK*2) == 0)){
            pll_clk_edge[i] = 1;
        }
        else {
            pll_clk_edge[i] = 0;
        }
    }

    



    // for (int i = 0;i<10000;i++){
    //     cout << output[i]; 
    //     if (i%10 == 0){
    //         cout << " " << endl;
    //     }
    // }
    // return 0;


}

    
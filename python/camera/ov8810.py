#!/usr/bin/python3

xclk = 27

pll_mult    = 44
pll_pre_div = 2
vt_sys_div  = 2
op_sys_div  = 8
op_pix_div  = 4
div8        = 5
rp_clk_div  = 4

sclk = xclk * pll_mult / pll_pre_div / div8 / vt_sys_div
pclk = sclk / rp_clk_div 
print(sclk)
print(pclk)
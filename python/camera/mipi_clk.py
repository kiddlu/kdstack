#!/usr/bin/python3

width=400
height=300

bits_per_pixel=16 #raw10

fps=30

hsync = 8
hfp = 0
hbp = 0
vsync = 6
vfp = 0
vbp = 0

mipi_lane_num = 2
ddr=2

hts = width + hsync + hfp + hbp
vts = height + vsync + vfp + vbp

total_bit_rate = hts * vts * bits_per_pixel * fps / 1000 / 1000
mipi_per_lane_rate =  total_bit_rate / mipi_lane_num
mipi_clock = mipi_per_lane_rate / ddr

print("total_bit_rate: " + str(total_bit_rate) + "Mbps")
print("mipi_per_lane_rate: " + str(mipi_per_lane_rate) + "Mbps")
print("mipi_clock: " + str(mipi_clock) + "MHz")

#!/bin/bash



	
# 	MAX=17
# export FUNCT=0
# while [ $FUNCT -lt 3 ]; do
#     export WIDTH=2
#     while [ $WIDTH -lt $MAX ]; do
# 	export WARE=0
# 	while [ $WARE -lt 2 ]; do
#             make powerTest.riscv
#             mv powerTest.riscv powerTest-$FUNCT-$WARE-$WIDTH.riscv
#             let WARE=$WARE+1
#         done
#         let WIDTH=$WIDTH*2
#     done
#     let FUNCT=$FUNCT+1
# done

# MAX=7
# FUNCT=4
# while [ $FUNCT -lt 7 ]; do
#     WIDTH=2
#     while [ $WIDTH -lt $MAX ]; do
# 	WARE=0
# 	while [ $WARE -lt 2 ]; do
#             make powerTest.riscv
#             mv powerTest.riscv powerTest-$FUNCT-$WARE-$WIDTH.riscv
#             let WARE=$WARE+1
#         done
#         let WIDTH=$WIDTH+2
#     done
#     let FUNCT=$FUNCT+1
# done

export FUNCT=2
export WIDTH=16
export WARE=0
make powerTest.riscv
mv powerTest.riscv powerTest-$FUNCT-$WARE-$WIDTH.riscv
WARE=1
make powerTest.riscv
mv powerTest.riscv powerTest-$FUNCT-$WARE-$WIDTH.riscv
export FUNCT=6
make powerTest.riscv
mv powerTest.riscv powerTest-$FUNCT-$WARE-$WIDTH.riscv
WARE=0
make powerTest.riscv
mv powerTest.riscv powerTest-$FUNCT-$WARE-$WIDTH.riscv

echo Made tests for functions up to $FUNCT-1 and widths up to $WIDTH/2

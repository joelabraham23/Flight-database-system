./testAss1 1 > output/1.out 
./testAss1 2 > output/2.out 
./testAss1 3 > output/3.out 
./testAss1 4 > output/4.out 
./testAss1 5 > output/5.out 
./testAss1 6 > output/6.out 
diff output/1.out output/1.exp
diff output/2.out output/2.exp
diff output/3.out output/3.exp
diff output/4.out output/4.exp
diff output/5.out output/5.exp
diff output/6.out output/6.exp

# ================================ Make all ================================
all: ex1 ex3_RMPO ex3_InvRMPO ex4_A ex5_checkTimes ex6 ex1_func2 ex3_RMPO_func2 ex3_InvRMPO_func2 ex4_A_func2  func2.o

# ============================== Using func.o ==============================
ex1: ex1.c func.o timestamps.c 
	sudo gcc -Wall -O2 -D_REENTRANT ex1.c func.o timestamps.c -o ex1 -lpthread -lrt

ex3_RMPO: ex3_RMPO.c func.o timestamps.c 
	sudo gcc -Wall -O2 -D_REENTRANT ex3_RMPO.c func.o timestamps.c -o ex3_RMPO -lpthread -lrt

ex3_InvRMPO: ex3_InvRMPO.c func.o timestamps.c 
	sudo gcc -Wall -O2 -D_REENTRANT ex3_InvRMPO.c func.o timestamps.c -o ex3_InvRMPO -lpthread -lrt

ex4_A: ex4_A.c func.o timestamps.c 
	sudo gcc -Wall -O2 -D_REENTRANT ex4_A.c func.o timestamps.c -o ex4_A -lpthread -lrt

ex5_checkTimes: ex5_checkTimes.c func.o timestamps.c 
	sudo gcc -Wall -O2 -D_REENTRANT ex5_checkTimes.c func.o timestamps.c -o ex5_checkTimes -lpthread -lrt

ex6: ex6.c func.o timestamps.c 
	sudo gcc -Wall -O2 -D_REENTRANT ex6.c func.o timestamps.c -o ex6 -lpthread -lrt

# ============================= Compile func2.c =============================
func2.o:
	sudo gcc -Wall -O2 -D_REENTRANT func2.c -c

# ============================== Using func2.o ==============================
ex1_func2: ex1.c func2.o timestamps.c 
	sudo gcc -Wall -O2 -D_REENTRANT ex1.c func2.o timestamps.c -o ex1_func2 -lpthread -lrt

ex3_RMPO_func2: ex3_RMPO.c func2.o timestamps.c 
	sudo gcc -Wall -O2 -D_REENTRANT ex3_RMPO.c func2.o timestamps.c -o ex3_RMPO_func2 -lpthread -lrt

ex3_InvRMPO_func2: ex3_InvRMPO.c func2.o timestamps.c 
	sudo gcc -Wall -O2 -D_REENTRANT ex3_InvRMPO.c func2.o timestamps.c -o ex3_InvRMPO_func2 -lpthread -lrt

ex4_A_func2: ex4_A.c func2.o timestamps.c 
	sudo gcc -Wall -O2 -D_REENTRANT ex4_A.c func2.o timestamps.c -o ex4_A_func2 -lpthread -lrt

# ================================== Clean ==================================
clean: 
	rm -f ex1 ex3_RMPO ex3_InvRMPO ex4_A ex5_checkTimes ex6 ex1_func2 ex3_RMPO_func2 ex3_InvRMPO_func2 ex4_A_func2  func2.o
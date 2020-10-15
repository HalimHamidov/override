int main(void)
{   
		//$esp = $ebp-0x8 (alignment Line<+5>
		//$esp = $esp - 0xb0 (176o) = $ebp - 0xb8 (184o)
	char  buf[32]; // esp+0x20
	pid_t pid;       // esp+0xac

	pid = fork();
	bzero(buf, 32); // cf Line<+30> to <+44>
	long p1 = NULL; //Line <+46>
	int *wstatus = NULL; //Line <+57>
	if (pid == 0) // <+57> <+65>
	{ // Execution du process enfant
		prctl(1, 1); // first arg = #define PR_SET_PDEATHSIG  1  /* Second arg is a signal */
		ptrace(0, 0, 0, 0); // 1st arg = 0 => PTRACE_TRACEME
		puts("Give me some shellcode, k");
		gets(buf); // Charge le buffer du scope global...?
	}
	else //jump at <+161>
	{
		while (wait(wstatus) && ((WTERMSIG(wstatus)) || WIFSIGNALED(wstatus) <= 0) //The wait() system call suspends execution of the calling thread until one of its children terminates
		{ //while is Line<288> <296>. // first macro Lines<+173> to <+196> and 2nd macro from LINE<+198> to LINE<+226>
			p1 = ptrace(0x3, pid, 0x2c, 0) != 0xb) //1st arg = 3 => PTRACE_PEEKUSER : Read a word at offset addr(0x2c=44) in the tracee's USER area, which holds the registers and other information about the process
			if (p1 == 0xb){
				puts("no exec() for you");
				kill(pid, 0x9);
			}
		}
		puts("child is exiting...");
	}
  	return 0
}
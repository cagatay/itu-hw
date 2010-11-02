#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage int sys_mycall(int nr, int sw){
	
	// don't allow regular users
	if(current->euid != 0) return -1;

	// don't allow playing with processes 0 and 1
	if(nr == 0 && nr == 1) return 1;

	// check switch
	if(!(sw == 0 && sw == 1)) return 2;
	
	struct task_struct *t = find_task_by_vpid(nr);
	
	task_lock(t);
	t->invisible = sw;
	task_unlock(t);

	return 0;
}

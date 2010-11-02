#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#define STACK_IOC_MAGIC 's'
#define STACK_POP _IO(STACK_IOC_MAGIC, 0)
#define STACK_SIZE 500

MODULE_LICENSE("GPL");

dev_t devno = 0;
int stack_major = 0;

struct Stack{
	char data[STACK_SIZE];
	int top;
};

struct StackDev{
	struct Stack data;
	struct cdev dev;
};

struct StackDev stack1, stack2;
struct cdev stack0;
struct proc_dir_entry *proc_entry;

void shiftr(struct Stack *s, int offset, int step){
	int i;
	int f = STACK_SIZE - step - 1;
	for(i = f; i >= offset; i--){
		s->data[i+step] = s->data[i];
	}
}

void shiftl(struct Stack *s, int offset){
	int i;
	int f = STACK_SIZE - 1;
	for(i = 0; i <= f; i++){
		s->data[i] = s->data[offset];
	}
}

void pop(struct Stack *s){
	int i;
	for(i=0; ; i++){
		if(s->data[i] == '\0')
			break;
	}
	shiftl(s, i);
	s->top-=i;
}

int stack_open(struct inode *inode, struct file *filp){
	filp->private_data = container_of(inode->i_cdev, struct StackDev, dev);
	return 0;
}

ssize_t stack_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos){
	struct StackDev *tmp = filp->private_data;
	
	if(*f_pos >= tmp->data.top)
		return 0;
	if(*f_pos + count >= tmp->data.top)
		count = tmp->data.top - *f_pos;
	copy_to_user(buf, tmp->data.data + *f_pos, count),
	*f_pos += count;
	return count;
}

ssize_t stack_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos){
	struct StackDev *tmp;
	tmp = filp->private_data;
	
	if(tmp->data.top >= STACK_SIZE)
		return 0;
	if(*f_pos >= STACK_SIZE)
		return 0;
	if(*f_pos + count >= STACK_SIZE)
		count = STACK_SIZE - *f_pos;
	if(*f_pos == 0){
		shiftr(&tmp->data, 0, 1);
		tmp->data.data[0] = '\0';
		tmp->data.top++;
	}
	shiftr(&tmp->data, *f_pos, count);
	copy_from_user(tmp->data.data + *f_pos, buf, count);
	
	*f_pos += count;
	tmp->data.top += count;
	
	return count;
}

int stack_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg){
	if(cmd == STACK_POP){
		if(stack1.data.top != 0){
			pop(&stack1.data);
			return 0;
		}
		if(stack2.data.top != 0){
			pop(&stack2.data);
			return 0;
		}
		return -1;
	}
	return -1;
}

int stack_proc_read( char *page, char **start, off_t off, int count, int * eof, void *data){
	int len = 0;
	int i;
	if(off >0){
		*eof = 1;
		return 0;
	}
	len += sprintf(page, "Stack1:\n");
	for(i=0; i< stack1.data.top; i++)
		len += sprintf(page, "%c", stack1.data.top);
	len += sprintf(page, "\nStack2:\n");
	for(i=0; i< stack2.data.top; i++)
		len += sprintf(page, "%c", stack2.data.top);
	return len;
}
	
struct file_operations stack_fops = {
	.owner = THIS_MODULE,
	.open = stack_open,
	.read = stack_read,
	.write = stack_write,
};

struct file_operations stack_fops0 = {
	.owner = THIS_MODULE,
	.ioctl = stack_ioctl,
};

int stack_init(void){
	int result;
	
	result = alloc_chrdev_region(&devno, 0, 3, "stack");
	stack_major = MAJOR(devno);
	
	cdev_init(&stack0, &stack_fops0);
	stack0.owner = THIS_MODULE;
	stack0.ops = &stack_fops0;
	cdev_add(&stack0, MKDEV(stack_major, 0), 1);
	
	cdev_init(&stack1.dev, &stack_fops);
	stack1.dev.owner = THIS_MODULE;
	stack1.dev.ops = &stack_fops;
	cdev_add(&stack1.dev, MKDEV(stack_major, 1), 1);

	cdev_init(&stack2.dev, &stack_fops);
	stack2.dev.owner = THIS_MODULE;
	stack2.dev.ops = &stack_fops;
	cdev_add(&stack2.dev, MKDEV(stack_major, 2), 1);
	
	proc_entry = create_proc_entry("stack", 0644, NULL);
	proc_entry->read_proc = stack_proc_read;
	return result;
}

void stack_exit(void){
	cdev_del(&stack0);
	cdev_del(&stack1.dev);
	cdev_del(&stack2.dev);
	unregister_chrdev_region(devno, 3);
	remove_proc_entry("stack", NULL);
}

module_init(stack_init);
module_exit(stack_exit);

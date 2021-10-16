#include <linux/list.h>
#include <linux/types.h>	// list_head
#include <linux/slab.h>		// kmalloc

// Core libraries
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>


struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

static LIST_HEAD(birthday_list);


int simple_init(void) {

	// Let's not make C90 sad with mixed declarations (:
	int i;
	struct birthday *ptr;
	struct birthday *person;

	// Create struct, allocate memory and initialize properties	
	person = kmalloc(sizeof(person), GFP_KERNEL);
	person->day = 1;
	person->month = 4;
	person->year = 2001;

	// Initialize head and Add the item to tail
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);


	// Add 5 more birthdays to the list	
	for (i = 0; i < 5; i++) {

		person = kmalloc(sizeof(person), GFP_KERNEL);
		person->day = i;
		person->month = i + 1;
		person->year = 2000 + i;

		// Add to the tail
		list_add_tail(&person->list, &birthday_list);
	}

	// Iterate 5 added birthdays and display their properties
	
	list_for_each_entry(ptr, &birthday_list, list) {
        printk(KERN_INFO "- birthdate: %d/%d/%d\n", ptr->year, ptr->month, ptr->day);
    }

    printk(KERN_INFO "Module Loaded.\n");
    return 0;
}

void simple_exit(void) {

	// Remofe items from list and free their allocated memory
	struct birthday *ptr;	

    list_for_each_entry(ptr, &birthday_list, list) {
    	kfree(ptr);
    }

    printk(KERN_INFO "Memory is free and Module is removed.\n");
}


// Macros for registering module entry and exit points
module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("How to add a linkedlist");
MODULE_AUTHOR("Keivan Ipchi Hagh");

/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Zherdiev Danylo <zerdevdanil430@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");
struct my_data {
	struct list_head list;
	ktime_t time;
};


static unsigned int number_of_iterations = 1;
module_param(number_of_iterations, uint, S_IRUGO);
MODULE_PARM_DESC(number_of_iterations, "Number of how many times 'Hello world' will be printed");

static LIST_HEAD(my_list_head);

static int __init hello_init(void)
{
	if (number_of_iterations == 0) {
		pr_warn("You printed 0 iterations\n");
	} else if (number_of_iterations >= 5 && number_of_iterations <= 10) {
		pr_warn("You printed between 5 and 10\n");
	}

	BUG_ON(number_of_iterations > 10);

	int counter = 0;
	while (number_of_iterations != counter) {
		struct my_data *my_data;
		if (counter == 3) {
			my_data = 0;
		} else {
			my_data = kmalloc(sizeof(*my_data), GFP_KERNEL);
		}

		my_data->time = ktime_get();
		list_add(&my_data->list, &my_list_head);
		pr_warn("Hello, world!\n");
		counter++;

	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct my_data *my_data,  *next;
	list_for_each_entry_safe(my_data, next, &my_list_head, list) {
		pr_info("Time: %lld nano seconds\n", ktime_to_ns(my_data->time));

		list_del(&my_data->list);

		kfree(my_data);

	}

}

module_init(hello_init);
module_exit(hello_exit);

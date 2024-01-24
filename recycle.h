
// test code
	node_printer(&head, &tail);
	push_back(&head, &tail, 4);
	printf("\n");
	node_printer(&head, &tail);
	printf("queue size : %d\n", q_size(&head));
	queue_clear(&head);
	printf("queue size : %d\n", q_size(&head));

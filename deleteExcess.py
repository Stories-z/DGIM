void DeleteExcess(pbucket h, int time, int windows) {
	pbucket pCur, pNext;
	pCur = h;
	pNext = pCur->next;
	
	while (pNext->next) {
		pCur = pCur->next;
		pNext = pCur->next;
	}
	if (pNext->timestamp < time - windows) {
		pCur->next = pNext->next;
		free(pNext);
	}
}

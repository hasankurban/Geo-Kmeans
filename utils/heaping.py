import numpy as np, heapq

def buildHeap(data,W,n,d,nclust): #build heap
	heaps = [[] for i in range(nclust)] #build min heap for each cluster
	minIndex = W.argmin(axis=0)  # predict the cluster
	for i in range(n):  #build the heaps
		heaps[minIndex[i]].append([W[minIndex[i],i],i])
	for h in range(nclust):  
		heapq.heapify(heaps[h])
	heaps,badDataIndex,badData,temp1 = seperateGoodBad(data,nclust,heaps,d)
	return heaps,badDataIndex,badData,temp1 
	 
def _heappush_min(heap, item):
	heap.append(item)
	heapq._siftdown(heap, 0, len(heap)-1)

def newHeap(data,heaps,W1,badDataIndex,d,nclust): #updating heap
	minIndex = W1.argmin(axis=0)
	for i in range(len(badDataIndex)): 
		_heappush_min(heaps[minIndex[i]],[W1[minIndex[i],i],badDataIndex[i][1]])
	heaps,badDataIndex,badData,temp2 =	seperateGoodBad(data,nclust,heaps,d)
	return heaps,badDataIndex,badData,temp2


def seperateGoodBad(data,nclust,heaps,d): # seperate good data and bad data

	leaves = [[] for i in range(nclust)]

	for l in range(nclust):
	   leaves[l] = heaps[l][int(len(heaps[l])/2):len(heaps[l])] #bad data
	   heaps[l] = heaps[l][0:int(len(heaps[l])/2)] #good data

	badDataIndex = sum(leaves, [])
	badDataIndexLen = len(badDataIndex)  
	badData = np.zeros((badDataIndexLen,d),dtype=float)
	temp =[]

	for j in range(badDataIndexLen):
		 badData[j] = data[badDataIndex[j][1]]
		 temp.append(badDataIndex[j][1])

	return heaps,badDataIndex,badData,temp
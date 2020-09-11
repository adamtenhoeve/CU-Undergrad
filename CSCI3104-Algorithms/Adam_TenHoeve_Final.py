import sys
import random
import networkx as nx


###########################################################################################################
# Problem 1 ###############################################################################################

def Merge(A, p, q, r, revs):
    B = [0] * len(A)
    i = p
    k = p
    j = q+1
    while i <= q and j <= r:
        if A[i] <= A[j]:
            B[k] = A[i]
            k+=1
            i+=1
        else:
            B[k] = A[j]
            k+=1
            j+=1
            revs[0]+= (j-k)
    while i <= q:
        B[k] = A[i]
        k+=1
        i+=1
    while j <= r:
        B[k] = A[j]
        k+=1
        j+=1
    i = p
    while i <= r:
        A[i] = B[i]
        i+=1

def findReverses(array, p, r, revs):
    if p < r:
        q = int((p + r) / 2)
        findReverses(array, p, q, revs)
        findReverses(array, q+1, r, revs)
        Merge(array, p, q, r, revs)

def prob1():
    print("########################################")
    print("Problem 1")
    n = input("Enter the number of elements in the array to be sorted: ")
    n = int(n)
    randomArray = [0] * n
    reverses = [0]
    for i in range(len(randomArray)):
        randomArray[i] = random.randint(1, 100)
    print ("Initial array is", randomArray)
    findReverses(randomArray, 0, len(randomArray)-1, reverses)
    print ("array sorted is ", randomArray, "and number of reverses is", reverses[0])

###########################################################################################################
# Problem 2 ###############################################################################################

def prob2():
    print("########################################################################################################")
    # Need li (min people per issue), ui (max people per issue), bp (min issues per person), tp (max issues per person)
    G = nx.DiGraph()
    d = 2 #demand for each vertex
    np = 1000 # Number of people
    ni = 10 # Number of issues
    li = random.randint(300, 400) # min people per issue
    ui = random.randint(500, 700) # max people per issue
    cap = 5 # capacity for each edge
    lower = 1 #lower bound on edges
    G.add_node("S")
    G.nodes["S"]['demand'] = 0
    G.add_node("t")
    G.nodes["t"]['demand'] = 0
    x = 0
    people = []
    issues = []
    while x < ni:
        issues.append("i"+str(x))
        G.add_node(issues[x])
        G.nodes[issues[x]]["demand"] = d
        x+=1
    x = 0
    while x < np:
        people.append("p"+str(x))
        G.add_node(people[x])
        G.nodes[people[x]]["demand"] = d
        G.nodes[people[x]]["hp"] = 0
        x+=1


    i = 0
    while i < np:
        j = 0
        while j < ni:
            opinion = random.choice([True, False])
            if opinion == True and G.has_edge(people[i], issues[j]) == False:
                if len(list(G.predecessors(issues[j]))) < ui:
                    G.add_edge(people[i], issues[j], capacity = 1)
                    G.nodes[people[i]]["demand"]+=lower
                    G.nodes[people[i]]["hp"]+= 1
                    G.nodes[issues[j]]["demand"]-=lower
            j+=1
        i+=1
        #if len(list(G.predecessors(people[i]))) < bp:
            #i-=1
    k = 0
    while k < ni:
        if len(list(G.predecessors(issues[k]))) < li:
            o = 0
            while o < np:
                if G.has_edge(people[o], issues[k]) == False:
                    if len(list(G.successors(people[o]))) < tp:
                        G.add_edge(people[o], issues[k], capacity = 1)
                        G.nodes[people[o]]["demand"]+=lower
                        G.nodes[people[o]]["hp"] += 1
                        G.nodes[issues[k]]["demand"]-=lower
                o+=1
        k+=1
        # if len(list(G.successors(issues[k]))) < li:
        #     k-=1
    x = 0
    while x < ni:
        G.add_edge(issues[x], "t", capacity = ui)#G.nodes[issues[x]]["demand"])
        x+=1
    x = 0
    while x < np:
        G.add_edge("S", people[x], capacity = G.nodes[people[x]]["hp"])#G.nodes[people[x]]["demand"])
        x+=1

    # for line in nx.generate_edgelist(G):
    #     print(line)
    #print(G.edges[data=True])
    print(nx.maximum_flow(G,"S","t",capacity='capacity'))

###########################################################################################################
# Problem 4 ###############################################################################################
def prob4():
    print("########################################")
    print("Problem 4")
    #create a directed graph
    G = nx.DiGraph()

    #adding an edge also adds the node
    G.add_edge('Spider', 'A', weight=1.0)
    G.add_edge('Spider', 'H', weight=1.0)
    G.add_edge('Spider', 'J', weight=1.0)

    G.add_edge('H', 'G', weight=1.0)
    G.add_edge('H', 'K', weight=1.0)

    G.add_edge('G', 'L', weight=1.0)
    G.add_edge('G', 'F', weight=1.0)

    G.add_edge('F', 'E', weight=1.0)

    G.add_edge('E', 'Fly', weight=1.0)

    G.add_edge('J', 'S', weight=1.0)
    G.add_edge('J', 'K', weight=1.0)

    G.add_edge('K', 'L', weight=1.0)
    G.add_edge('L', 'M', weight=1.0)
    G.add_edge('M', 'N', weight=1.0)
    G.add_edge('M', 'F', weight=1.0)

    G.add_edge('N', 'O', weight=1.0)
    G.add_edge('N', 'E', weight=1.0)

    G.add_edge('O', 'Fly', weight=1.0)

    G.add_edge('A', 'S', weight=1.0)
    G.add_edge('A', 'B', weight=1.0)

    G.add_edge('B', 'R', weight=1.0)
    G.add_edge('B', 'C', weight=1.0)

    G.add_edge('S', 'R', weight=1.0)
    G.add_edge('R', 'Q', weight=1.0)

    G.add_edge('Q', 'C', weight=1.0)
    G.add_edge('Q', 'P', weight=1.0)

    G.add_edge('C', 'D', weight=1.0)
    G.add_edge('D', 'Fly', weight=1.0)
    G.add_edge('P', 'D', weight=1.0)
    G.add_edge('P', 'O', weight=1.0)
    G.add_edge('O', 'Fly', weight=1.0)

    G.add_edge('T', 'Q', weight=1.0)
    G.add_edge('T', 'P', weight=1.0)
    G.add_edge('T', 'O', weight=1.0)
    G.add_edge('T', 'N', weight=1.0)
    G.add_edge('T', 'M', weight=1.0)

    G.add_edge('R', 'T', weight=1.0)
    G.add_edge('S', 'T', weight=1.0)
    G.add_edge('J', 'T', weight=1.0)
    G.add_edge('K', 'T', weight=1.0)
    G.add_edge('L', 'T', weight=1.0)

    #each edge has a weight of 1. The shortest path is the fewest edges.
    #Use this to verify that your graph built correctly.
    t = nx.shortest_path(G, 'Spider', 'Fly', weight='weight')

    print("The shortest path in graph G is", t)

    #Algorithm to find the total number of paths

    stack = []
    stack.append("Spider")
    for i in G:
        G.nodes[i]['paths'] = 0
    G.nodes["Spider"]['paths'] = 1
    while len(stack) != 0:
        temp = stack.pop() # string corresponding to a node
        succ = G.successors(temp)
        S = list(succ)
        for i in S:
            stack.append(i)
        pred = G.predecessors(temp)
        P = list(pred)
        if temp == "Spider":
            G.nodes["Spider"]['paths'] = 1
        else:
            PathsToNodes = 0
            for i in P:
                PathsToNodes += G.nodes[i]['paths']
            G.nodes[temp]['paths'] = PathsToNodes
    print ("The total number of paths in graph G is", G.nodes["Fly"]['paths'])


###########################################################################################################
# Problem 5 ###############################################################################################

# mergeSort Algorithm for if you want to return true or false that someone doesn't get hit
# def Merge(A, p, q, r):
#     B = [0] * len(A)
#     i = p
#     k = p
#     j = q+1
#     while i <= q and j <= r:
#         if A[i] <= A[j]:
#             B[k] = A[i]
#             k+=1
#             i+=1
#         else:
#             B[k] = A[j]
#             k+=1
#             j+=1
#     while i <= q:
#         B[k] = A[i]
#         k+=1
#         i+=1
#     while j <= r:
#         B[k] = A[j]
#         k+=1
#         j+=1
#     i = p
#     while i <= r:
#         A[i] = B[i]
#         i+=1
#
# def mergeSort(array, p, r):
#     if p < r:
#         q = int((p + r) / 2)
#         mergeSort(array, p, q)
#         mergeSort(array, q+1, r)
#         Merge(array, p, q, r)

def isHit(matrix, n):
    array = [0] * n
    for i in list(range(n)):
        dist = 100000
        for j in list(range(n)):
            # print ("i is", i, "and j is", j)
            if i != j:
                if matrix[i][j] < dist:
                    # print ("dist was ", dist)
                    # print("matrix[",i,"][",j,"] is", matrix[i][j], "and is new dist")
                    dist = matrix[i][j]
                    array[i] = j
    return array
    # Do this if you want to return true or false that there remains someone who is never hit
    # mergeSort(array, 0, n-1)
    # for i in list(range(n)):
    #     if array[i] != i:
    #         return True
    # return False

def prob5():
    print("########################################")
    print("Problem 5")
    n = 7
    #Create a matrix of distances between two people
    matrix = [[0 for x in range(n)] for y in range(n)]
    for i in list(range(n)):
        for j in range(i,n):
            x = random.randint(1,100)
            matrix[i][j] = x
            matrix[j][i] = x
    for i in list(range(n)):
        matrix[i][i] = 0
    print("Matrix of distances between people:")
    for i in range(n):
        print (matrix[i])
    print ("A list of who hit who:", isHit(matrix, n))


###########################################################################################################
###########################################################################################################

prob1()
prob2()
prob4()
prob5()

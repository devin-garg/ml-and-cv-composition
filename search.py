# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util
import heapq, random
import searchAgents
class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print "Start:", problem.getStartState()
    print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    print "Start's successors:", problem.getSuccessors(problem.getStartState())
    """
    closed = set([])
    fringe = util.Stack()
    fringe.push((problem.getStartState(),''))
    expandingGraph = {}
    while True:
        if fringe.isEmpty():
            return []
        node = fringe.pop()
        if problem.isGoalState(node[0]):
            directionList = []
            while node != None:
                directionList.append(node[1])
                if node in expandingGraph.keys() and expandingGraph[node][0] != problem.getStartState():
                    node = expandingGraph[node]
                else:
                    node = None
            directionList.reverse()
            return directionList
        if node[0] not in closed:
            closed.add(node[0]) 
            succeedingChildren = problem.getSuccessors(node[0])
            for successorNode in succeedingChildren:
                if successorNode[0] not in closed:
                    expandingGraph[successorNode] = node
                fringe.push(successorNode)


def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    "*** YOUR CODE HERE ***"
    closed = set([])
    fringe = util.Queue()
    fringe.push((problem.getStartState(),''))
    expandingGraph = {}
    while True:
        if fringe.isEmpty():
            return []
        node = fringe.pop()
        if problem.isGoalState(node[0]):
            directionList = []
            while node != None:
                directionList.append(node[1])
                if node in expandingGraph.keys() and expandingGraph[node][0] != problem.getStartState():
                    node = expandingGraph[node]
                else:
                    node = None
            directionList.reverse()
            return directionList
        if node[0] not in closed:
            closed.add(node[0]) 
            succeedingChildren = problem.getSuccessors(node[0])
            for successorNode in succeedingChildren:
                if successorNode[0] not in closed:
                    expandingGraph[successorNode] = node
                fringe.push(successorNode)


def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    "*** YOUR CODE HERE ***"
    closed = set([])
    fringe = ModifiedPriorityQueue()
    fringe.push((problem.getStartState(), ""), 0, 0)
    expandingGraph = {}
    while True:
        if fringe.isEmpty():
            return []
        node = fringe.pop()
        costNode = node[1]
        node = node[0]
        if problem.isGoalState(node[0]):
            directionList = []
            while node != None:
                directionList.append(node[1])
                if node in expandingGraph.keys() and expandingGraph[node][0] != problem.getStartState():
                    node = expandingGraph[node]
                else:
                    node = None
            directionList.reverse()
            return directionList
        if node[0] not in closed:
            closed.add(node[0]) 
            succeedingChildren = problem.getSuccessors(node[0])
            for successorNode in succeedingChildren:
                if successorNode[0] not in closed:
                    expandingGraph[successorNode] = node
                fringe.push(successorNode, successorNode[2] + costNode, 0)

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    "*** YOUR CODE HERE ***"
    closed = set([])
    fringe = ModifiedPriorityQueue()
    fringe.push((problem.getStartState(), ""), 0, 0)
    expandingGraph = {}
    while True:
        if fringe.isEmpty():
            return []
        node = fringe.pop()
        costNode = node[1]
        node = node[0]
        if problem.isGoalState(node[0]):
            directionList = []
            while node != None:
                directionList.append(node[1])
                if node in expandingGraph.keys() and expandingGraph[node][0] != problem.getStartState():
                    node = expandingGraph[node]
                else:
                    node = None
            directionList.reverse()
            return directionList
        if node[0] not in closed:
            closed.add(node[0]) 
            succeedingChildren = problem.getSuccessors(node[0])
            for successorNode in succeedingChildren:
                if successorNode[0] not in closed:
                    expandingGraph[successorNode] = node
                fringe.push(successorNode, successorNode[2] + costNode, heuristic(successorNode[0], problem))
  


# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch

class ModifiedPriorityQueue:
    """
      Implements a priority queue data structure. Each inserted item
      has a priority associated with it and the client is usually interested
      in quick retrieval of the lowest-priority item in the queue. This
      data structure allows O(1) access to the lowest-priority item.

      Note that this PriorityQueue does not allow you to change the priority
      of an item.  However, you may insert the same item multiple times with
      different priorities.

      I modified to store the heuristicCost in the queue and priority to be the sum of edgeCost and heuristicCost. 
      On pop() I return the Item and the edgeCost.
    """
    def  __init__(self):
        self.heap = []
        self.count = 0

    def push(self, item, edgeCost, heuristicCost):
        # FIXME: restored old behaviour to check against old results better
        # FIXED: restored to stable behaviour
        entry = (edgeCost + heuristicCost, self.count, item, heuristicCost)
        # entry = (priority, item)
        heapq.heappush(self.heap, entry)
        self.count += 1

    def pop(self):
        (costItem, _, item, heuristicCost) = heapq.heappop(self.heap)
        #  (_, item) = heapq.heappop(self.heap)
        return [item, costItem - heuristicCost]

    def isEmpty(self):
        return len(self.heap) == 0

# multiAgents.py
# --------------
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


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"
        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]
        "*** YOUR CODE HERE ***"
        # I will be finding the minimum distance to the food via manhattan distance
        minimumDistanceToFood = float("inf")
        for foodPellet in newFood.asList():
          myManhattanDist = manhattanDistance(newPos, foodPellet)
          if myManhattanDist < minimumDistanceToFood:
            minimumDistanceToFood = myManhattanDist

        # Only positions where the ghost is within a specified threshold manhattan distance wise will increase ghostDangerAdder.
        # I chose 4 for this threshold
        ghostDangerAdder = 0
        thresholdForGhostDist = 4
        for ghostPosition in [s.getPosition() for s in newGhostStates]:
          myManhattanDistForGhost = manhattanDistance(newPos, ghostPosition)
          if thresholdForGhostDist > myManhattanDistForGhost:
            ghostDangerAdder += thresholdForGhostDist - myManhattanDistForGhost

        # add the effects of recieprocal to nearest food and negate the danger of the ghost
        return successorGameState.getScore() - ghostDangerAdder + 1.0/minimumDistanceToFood

def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.

          Here are some method calls that might be useful when implementing minimax.

          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1

          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action

          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"


        def value(state, agentNum, depthOn):
          if gameState.getNumAgents() == agentNum:
            agentNum = 0
            depthOn += 1
          if depthOn >= self.depth  or state.isLose() or state.isWin():
            return self.evaluationFunction(state)
          if agentNum == 0:
            return  max_value(state, agentNum, depthOn)
          else:
            return min_value(state, agentNum, depthOn)
        def max_value(state, agentNum, depthOn):
          v = -float("inf")
          optimalMove = None
          for actionPossible in state.getLegalActions(agentNum):
            if depthOn == 0:
              myVal = value(state.generateSuccessor(agentNum, actionPossible), agentNum + 1, depthOn)
              if myVal > v:
                v = myVal
                optimalMove = actionPossible
            else:
              v = max(v, value(state.generateSuccessor(agentNum, actionPossible), agentNum + 1, depthOn))

          return v if depthOn != 0 else optimalMove

        def min_value(state, agentNum, depthOn):
          v = float("inf")
          for actionPossible in state.getLegalActions(agentNum):
            v = min(v, value(state.generateSuccessor(agentNum, actionPossible), agentNum + 1, depthOn))
          return v          

        return value(gameState, 0, 0)


class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        def value(state, agentNum, depthOn, alphaVal, betaVal):
          if gameState.getNumAgents() == agentNum:
            agentNum = 0
            depthOn += 1
          if depthOn >= self.depth  or state.isLose() or state.isWin():
            return self.evaluationFunction(state)
          if agentNum == 0:
            return  max_value(state, agentNum, depthOn, alphaVal, betaVal)
          else:
            return min_value(state, agentNum, depthOn, alphaVal, betaVal)
        def max_value(state, agentNum, depthOn, alphaVal, betaVal):
          v = -float("inf")
          optimalMove = None
          for actionPossible in state.getLegalActions(agentNum):
            myVal = value(state.generateSuccessor(agentNum, actionPossible), agentNum + 1, depthOn, alphaVal, betaVal)
            if depthOn == 0:
              if myVal > v:
                v = myVal
                optimalMove = actionPossible
            else:
              v = max(v, myVal)
            alphaVal = max(alphaVal, v)
            if betaVal < alphaVal:
              break

          return v if depthOn != 0 else optimalMove

        def min_value(state, agentNum, depthOn, alphaVal, betaVal):
          v = float("inf")
          for actionPossible in state.getLegalActions(agentNum):
            v = min(v, value(state.generateSuccessor(agentNum, actionPossible), agentNum + 1, depthOn, alphaVal, betaVal))
            betaVal = min(v, betaVal)
            if betaVal < alphaVal:
              break
          return v          

        return value(gameState, 0, 0, -float('inf'), float('inf'))


class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
          Returns the expectimax action using self.depth and self.evaluationFunction

          All ghosts should be modeled as choosing uniformly at random from their
          legal moves.
        """
        "*** YOUR CODE HERE ***"
        def value(state, agentNum, depthOn):
          if gameState.getNumAgents() == agentNum:
            agentNum = 0
            depthOn += 1
          if depthOn >= self.depth  or state.isLose() or state.isWin():
            return self.evaluationFunction(state)
          if agentNum == 0:
            return  max_value(state, agentNum, depthOn)
          else:
            return exp_value(state, agentNum, depthOn)
        def max_value(state, agentNum, depthOn):
          v = -float("inf")
          optimalMove = None
          for actionPossible in state.getLegalActions(agentNum):
            if depthOn == 0:
              myVal = value(state.generateSuccessor(agentNum, actionPossible), agentNum + 1, depthOn)
              if myVal > v:
                v = myVal
                optimalMove = actionPossible
            else:
              v = max(v, value(state.generateSuccessor(agentNum, actionPossible), agentNum + 1, depthOn))

          return v if depthOn != 0 else optimalMove

        def exp_value(state, agentNum, depthOn):
          v = 0
          legalMoves = state.getLegalActions(agentNum)
          for actionPossible in legalMoves:
            v +=  (1.0/len(legalMoves))* value(state.generateSuccessor(agentNum, actionPossible), agentNum + 1, depthOn)
          return v          

        return value(gameState, 0, 0)

def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: I wanted to incorporate the effects of minimum distance to food, ghost, current score, number of food left, and the number of capsules remaining.
      Looking at the return value, if there was a ghost within the threshold of 2 manhattan distance wise (threshold gotten through testing), I returned the worst 
      evalution of -infinity. I looked at the minimum distance to the nearest food item if exists and negated then divided by 10.0 otherwise added 0 to the returned value.
      I added in the current game score then subtracted by the number of food available with weight 100 and then I subtracted by the number of capsules left. 
      The weighting of the minimum distance manhattan distance wise and the number of food left were achieved through testing.
    """
    "*** YOUR CODE HERE ***"

    pacmanPos = currentGameState.getPacmanPosition()
    minDist = float("inf")
    for foodPos in currentGameState.getFood().asList():
        minDist = min(minDist, manhattanDistance(pacmanPos, foodPos))
    for ghostPos in currentGameState.getGhostPositions():
        if manhattanDistance(pacmanPos, ghostPos) < 2:
            return -float("inf")
    return (-minDist/10.0 if minDist != float("inf") else 0) + currentGameState.getScore() - 100*currentGameState.getNumFood() - len(currentGameState.getCapsules())

# Abbreviation
better = betterEvaluationFunction


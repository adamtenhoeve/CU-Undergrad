package jsy.student

import jsy.lab1._

object Lab1 extends jsy.util.JsyApplication with jsy.lab1.Lab1Like {
  import jsy.lab1.Parser
  import jsy.lab1.ast._

  /*
   * CSCI 3155: Lab 1
   * Adam Ten Hoeve
   *
   * Partner: Matt Waymouth
   * Collaborators: <Any Collaborators>
   */

  /*
   * Fill in the appropriate portions above by replacing things delimited
   * by '<'... '>'.
   *
   * Replace the '???' expression with your code in each function. The
   * '???' expression is a Scala expression that throws a NotImplementedError
   * exception.
   *
   * Do not make other modifications to this template, such as
   * - adding "extends App" or "extends Application" to your Lab object,
   * - adding a "main" method, and
   * - leaving any failing asserts.
   *
   * Your lab will not be graded if it does not compile.
   *
   * This template compiles without error. Before you submit comment out any
   * code that does not compile or causes a failing assert.  Simply put in a
   * '???' as needed to get something that compiles without error.
   */

  /*
   * Example: Test-driven development of plus
   *
   * A convenient, quick-and-dirty way to experiment, especially with small code
   * fragments, is to use the interactive Scala interpreter. The simplest way
   * to use the interactive Scala interpreter in IntelliJ is through a worksheet,
   * such as Lab1Worksheet.sc. A Scala Worksheet (e.g., Lab1Worksheet.sc) is code
   * evaluated in the context of the project with results for each expression
   * shown inline.
   *
   * Step 0: Sketch an implementation in Lab1.scala using ??? for unimmplemented things.
   * Step 1: Do some experimentation in Lab1Worksheet.sc.
   * Step 2: Write a test in Lab1Spec.scala, which should initially fail because of the ???.
   * Step 3: Fill in the ??? to finish the implementation to make your test pass.
   */

  //def plus(x: Int, y: Int): Int = ???
  def plus(x: Int, y: Int): Int = x + y

  /* Exercises */

  def abs(n: Double): Double = {
    if (n > 0) // If positive then returns the same number
      n
    else // If negative then returns negative n to get the absolute value
      -n
  }

  def xor(a: Boolean, b: Boolean): Boolean = // Check for exclusive or
  {
    if (a == b) // If both values are the same (both true or both false), then returns false
      false
    else // Otherwise one is true and the other is false, meaning xor returns true
      true
  }

  def repeat(s: String, n: Int): String = { // Turns a word s repeated n times
    require(n >= 0) // Checks to make sure n is not negative
    var word:String = "" // Initialize empty string
    if (n > 0) // Base case for recursion, will not enter for loop after n iterations
      {
        word = s + repeat(s, n-1) // Adds word s to a recursive call of the function for n-1 more times
      }
    word // Returns the final summed word of s repeated n times
  }

  def sqrtStep(c: Double, xn: Double): Double = {
    require(c > 0) 
    xn - ((xn*xn-c) / (2 * xn)) // Computes the square root from the square root formula
  }

  def sqrtNhelper(c: Double, x: Double, n: Int) :Double = { // Helper function for sqrtN
    if (n > 0)
      sqrtNhelper(c, sqrtStep(c, x), n-1) // Recursively calls the helper function with n-1 as iterations so it executes
    else                                  // the sqrtStep function a total of n times
      x     // When done executing, returns the final calculated value
  }

  def sqrtN(c: Double, x0: Double, n: Int): Double = {
    require(c > 0)
    require(n >= 0) // will not run if negative iterations
    if (n == 0) x0 // returns value if no iterations
    sqrtNhelper(c, x0, n) // calls helper function n times, which returns calculated value
  }

  def sqrtErr(c: Double, x0: Double, epsilon: Double): Double = { // Calculates sqrt until within certain error
    require(epsilon > 0) // Exits once within certain error. Stops function from running infinitely
    if (abs((x0 * x0) - c) >= epsilon) // If not within the error, recursively calls the function again with calculated parameters
    {
      sqrtErr(c, sqrtStep(c, x0), epsilon) // Uses sqrtStep to calculate square root after each iteration
    }
    else // returns final value
    {
      x0
    }
  }

  def sqrt(c: Double): Double = {
    require(c >= 0)
    if (c == 0) 0 else sqrtErr(c, 1.0, 0.0001)
  }

  /* Search Tree */

  // Defined in Lab1Like.scala:
  //
  // sealed abstract class SearchTree
  // case object Empty extends SearchTree
  // case class Node(l: SearchTree, d: Int, r: SearchTree) extends SearchTree

  def repOk(t: SearchTree): Boolean = { // Function to see if tree is valid
    def check(t: SearchTree, min: Int, max: Int): Boolean = t match { // checks to see if each node is valid
      case Empty => true // Returns if empty node
      case Node(l, d, r) => if (d >= min && d < max) // if is true if node is correct in tree, no problems
        check(l, min, d) && check (r, d, max) // calls on children with updated parameters
      else
        false // returns false if tree has some incorrect parameters
    }
    check(t, Int.MinValue, Int.MaxValue)
  }

  def insert(t: SearchTree, n: Int): SearchTree = t match{ // Function to add a node to bottom of tree
    case Empty => Node(Empty, n, Empty) // If bottom of tree, adds node to bottom
    case Node(l, d, r) => // If not bottom, then must traverse tree
      if (n < d) Node(insert(l, n), d, r) // If n < d, goes down left subtree. Always returns a node
      else Node(l, d, insert(r, n)) // else traverses down right subtree
  }

  def deleteMin(t: SearchTree): (SearchTree, Int) = { // Function to find minimum node of right subtree
    require(t != Empty)
    (t: @unchecked) match {
      case Node(Empty, d, r) => (r, d) // If no left child, then is minimum value and returns that value and right tree of that value
      case Node(l, d, r) => // If not minimum value, continues to traverse right.
        val (l1, m) = deleteMin(l) // l1 is right subtree of minimum value, m is minimum value
        (Node(l1, d, r), m) // returns the touple of the tree with that value replaced and the value that was deleted
    }
  }

  def delete(t: SearchTree, n: Int): SearchTree = // Function to delete any node on the tree
  {
    (t: @unchecked) match {
      case Node(Empty, d, Empty) => // No children
        if (n != d) // If not equal to the value, then not in tree
          t // Returns full tree
        else // Node to be deleted has no children as can be set to Empty
          Empty
      case Node(l, d, Empty) => // Only left child
        if (n == d) // If value
          l // Set Node as left subtree
        else // Not equal to value
          Node(delete(l, n), d, Empty) // Continue traversing left
      case Node(Empty, d, r) => // Only right child
        if (n == d) { // if value
          r // Overwrite Node with right subtree
        }
        else // Not value
          Node(Empty, d, delete(r, n)) // Traverse down right subtree
      case Node(l, d, r) => // Two children
        if (n == d) // if value
        {
          val (tree, value) = deleteMin(r) // call deleteMin to find minimum value of right subtree
          Node(l, value, tree) // Sets returns current node as the minimum value, deleting current node
        }
        else if (n < d) // If n is less than current node, traverses down left subtree
          Node(delete(l, n), d, r)
        else  // If greater than, traverses down right subtree.
          Node(l, d, delete(r, n))
    }
  }

  /* JavaScripty */

  def eval(e: Expr): Double = e match { // Basic calculator function to calculate simple expressions
    case N(n) => n // If expression is a number, returns the number
      n
    case Unary(uop, e1) => uop match { // If expression is a single operator, returns the operator
      case Neg => -1 * eval(e1) // Only operator is negative so multiplies the value by -1
    }
    case Binary(bop, e1, e2) => bop match { // If expression is binary, case matches which binary expression and performs calulation
      case Plus => eval(e1) + eval(e2) // addition
      case Minus => eval(e1) - eval(e2) // subtraction
      case Times => eval(e1) * eval(e2) // multiplication
      case Div => eval(e1) / eval(e2) // division
                                      // Calls eval on each subexpression in case abstract tree is longer
    }
  }

 // Interface to run your interpreter from a string.  This is convenient
 // for unit testing.
 def eval(s: String): Double = eval(Parser.parse(s))



 /* Interface to run your interpreter from the command-line.  You can ignore the code below. */

 def processFile(file: java.io.File) {
    if (debug) { println("Parsing ...") }

    val expr = Parser.parseFile(file)

    if (debug) {
      println("\nExpression AST:\n  " + expr)
      println("------------------------------------------------------------")
    }

    if (debug) { println("Evaluating ...") }

    val v = eval(expr)

    println(prettyNumber(v))
  }

}

package jsy.student

import jsy.lab2.Lab2Like

object Lab2 extends jsy.util.JsyApplication with Lab2Like {
  import jsy.lab2.Parser
  import jsy.lab2.ast._

  /*
   * CSCI 3155: Lab 2
   * Adam Ten Hoeve
   * 
   * Partner: Nihar Nandan
   * Collaborators: Dylan Fox
   */

  /*
   * Fill in the appropriate portions above by replacing things delimited
   * by '<'... '>'.
   * 
   * Replace the '???' expression with  your code in each function.
   * 
   * Do not make other modifications to this template, such as
   * - adding "extends App" or "extends Application" to your Lab object,
   * - adding a "main" method, and
   * - leaving any failing asserts.
   * 
   * Your lab will not be graded if it does not compile.
   * 
   * This template compiles without error. Before you submit comment out any
   * code that does not compile or causes a failing assert. Simply put in a
   * '???' as needed to get something  that compiles without error. The '???'
   * is a Scala expression that throws the exception scala.NotImplementedError.
   *
   */

  /* We represent a variable environment as a map from a string of the
   * variable name to the value to which it is bound.
   * 
   * You may use the following provided helper functions to manipulate
   * environments, which are just thin wrappers around the Map type
   * in the Scala standard library.  You can use the Scala standard
   * library directly, but these are the only interfaces that you
   * need.
   */



  /* Some useful Scala methods for working with Scala values include:
   * - Double.NaN
   * - s.toDouble (for s: String)
   * - n.isNaN (for n: Double)
   * - n.isWhole (for n: Double)
   * - s (for n: Double)
   * - s format n (for s: String [a format string like for printf], n: Double)
   *
   * You can catch an exception in Scala using:
   * try ... catch { case ... => ... }
   */

  def toNumber(v: Expr): Double = {
    require(isValue(v))
    (v: @unchecked) match {
      case N(n) => n
      case B(b) => if (b) 1.0 else 0.0 // if true return 1, if false return 0
      case S("")=> 0.0 // if empty string, return 0
      case S(s) =>
        try {s.toDouble}
        catch {case e: Exception => Double.NaN}
      case Undefined => Double.NaN
      case _ => Double.NaN;
    }
  }

  def toBoolean(v: Expr): Boolean = {
    require(isValue(v))
    (v: @unchecked) match {
      case B(b) => b
      case N(n) =>
        if(n == 0.0 || n.isNaN) false
        else true
      case S("") => false
      case S(s) => true
      case Undefined => false
      case _ => false
    }
  }

  def toStr(v: Expr): String = {
    require(isValue(v))
    (v: @unchecked) match {
      case S(s) => s
      case Undefined => "undefined"
      case N(n) => try{pretty(v)} catch {case e: Exception => ""}
      case B(b) => pretty(v)
      case _ => ""
    }
  }

  def eval(env: Env, e: Expr): Expr = {
    e match {
      /* Base Cases */
      case N(n) => N(n)
      case S(s) => S(s)
      case B(b) => B(b)
      case Undefined => Undefined
      /* Inductive Cases */
      case Var(x) => lookup(env, x)
      case ConstDecl(x, e1, e2) => {
        val v = eval(env, e1)
        val env2 = extend(env, x, v)
        eval(env2, e2)
      }
      case Print(e1) => println(pretty(eval(env, e1))); Undefined
      case Unary(uop, e1) =>
        uop match {
          case Neg => N(-toNumber(e1))
          case Not => B(!toBoolean(e1))
        }
      case Binary(And, e1, e2) => {
        val first = eval(env, e1)
        if (toBoolean(first)) eval(env, e2) else first
      } // If first expression is true, return the second expression. Else return the first.
      case Binary(Or, e1, e2) => {
        val first = eval(env, e1)
        if (toBoolean(first)) first else eval(env, e2)
      } // Return first if first expression is true, else return false
      case Binary(bop, e1, e2) =>
        val first  = eval(env, e1)
        val second = eval(env, e2)
        bop match {
          case Plus => (first, second) match {
            case (S(first), _) => S(first + toStr(second)) // left expression is string
            case (_, S(second)) => S(toStr(first) + second) // right expression is string
            case _ => N(toNumber(first) + toNumber(second)) // neither expression is string
          }
          case Minus => N(toNumber(first) - toNumber(second)) // eval(env, e1) - eval(env, e2)
          case Times => N(toNumber(first) * toNumber(second)) // eval(env, e1) * eval(env, e2)
          case Div => N(toNumber(first) / toNumber(second)) // eval(env, e1) / eval(env, e2)
          case Eq => B(if (first == second) true else false) // eval(env, e1) === eval(env, e2)
          case Ne => B(if (first == second) false else true) // eval(env, e1) !== eval(env, e2)
          case Lt => (first, second) match { // eval(env, e1) < eval(env, e2)
            case (S(s1), S(s2)) => B(if (s1 < s2) true else false)
            case _ => B(if (toNumber(first) < toNumber(second)) true else false)
          }
          case Le => (first, second) match { // eval(env, e1) <= eval(env, e2)
            case (S(s1), S(s2)) => B(if (s1 <= s2) true else false)
            case _ => B(if (toNumber(first) <= toNumber(second)) true else false)
          }
          case Gt => (first, second) match {// eval(env, e1) > eval(env, e2)
            case (S(s1), S(s2)) => B(if (s1 > s2) true else false)
            case _ => B(if (toNumber(first) > toNumber(second)) true else false)
          }
          case Ge => (first, second) match {
            case (S(s1), S(s2)) => B(if (s1 >= s2) true else false)
            case _ => B(if (toNumber(first) >= toNumber(second)) true else false)
          } // eval(env, e1) >= eval(env, e2)
//          case And => if (toBoolean(first)) second else first // If first expression is true, return the second expression. Else return the first.
//          case Or => if (toBoolean(first)) first else second // Return first if first expression is true, else return false
          case Seq => second // eval(env, e1), eval(env, e2)
        }
      case If(e1, e2, e3) =>
        if (toBoolean(eval(env, e1))) eval(env, e2) else eval(env, e3) // e1? then e2 else e3
      case _ => e
    }
  }



  /* Interface to run your interpreter from the command-line.  You can ignore what's below. */
  def processFile(file: java.io.File) {
    if (debug) { println("Parsing ...") }

    val expr = Parser.parseFile(file)

    if (debug) {
      println("\nExpression AST:\n  " + expr)
      println("------------------------------------------------------------")
    }

    if (debug) { println("Evaluating ...") }

    val v = eval(expr)

     println(pretty(v))
  }

}

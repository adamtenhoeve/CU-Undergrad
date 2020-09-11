package jsy.student

import jsy.lab3.Lab3Like
import jsy.util.JsyApplication

object Lab3 extends JsyApplication with Lab3Like {
  import jsy.lab3.ast._
  
  /*
   * CSCI 3155: Lab 3 
   * Adam Ten Hoeve
   * 
   * Partner: Cassie
   * Collaborators: <Any Collaborators>
   */

  /*
   * Fill in the appropriate portions above by replacing things delimited
   * by '<'... '>'.
   * 
   * Replace the '???' expression with your code in each function.
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
   */
  
  /*
   * The implementations of these helper functions for conversions can come
   * Lab 2. The definitions for the new value type for Function are given.
   */
  
  def toNumber(v: Expr): Double = {
    require(isValue(v))
    (v: @unchecked) match {
      case N(n) => n
      case B(false) => 0
      case B(true) => 1
      case Undefined => Double.NaN
      case S(s) => {
        try {s.toDouble}
        catch {case e: Exception => Double.NaN}
      }
      case Function(_, _, _) => Double.NaN
    }
  }
  
  def toBoolean(v: Expr): Boolean = {
    require(isValue(v))
    (v: @unchecked) match {
      case B(b) => b
      case Function(_, _, _) => true
      case N(n) => {
        if (n == 0.0 || n.isNaN) false
        else true
      }
      case S("") => false
      case S(s) => true
      case Undefined => false
    }
  }
  
  def toStr(v: Expr): String = {
    require(isValue(v))
    (v: @unchecked) match {
      case S(s) => s
        // Here in toStr(Function(_, _, _)), we will deviate from Node.js that returns the concrete syntax
        // of the function (from the input program).
      case Function(_, _, _) => "function"
      case Undefined => "undefined" // delete this line when done
      case N(n) => try {pretty(v)} catch {case e: Exception => ""}
      case B(b) => pretty(v)
      case _ => ""
    }
  }

  /*
   * Helper function that implements the semantics of inequality
   * operators Lt, Le, Gt, and Ge on values.
   *
   * We suggest a refactoring of code from Lab 2 to be able to
   * use this helper function in eval and step.
   */
  def inequalityVal(bop: Bop, v1: Expr, v2: Expr): Boolean = {
    require(isValue(v1))
    require(isValue(v2))
    require(bop == Lt || bop == Le || bop == Gt || bop == Ge)
    bop match {
      case Lt => (v1, v2) match {
        case (S(s1), S(s2)) => if (s1 < s2) true else false
        case _ => if (toNumber(v1) < toNumber(v2)) true else false
      }
      case Le => (v1, v2) match {
        case (S(s1), S(s2)) => if (s1 <= s2) true else false
        case _ => if (toNumber(v1) <= toNumber(v2)) true else false
      }
      case Gt => (v1, v2) match {
        case (S(s1), S(s2)) => if (s1 > s2) true else false
        case _ => if (toNumber(v1) > toNumber(v2)) true else false
      }
      case Ge => (v1, v2) match {
        case (S(s1), S(s2)) => if (s1 >= s2) true else false
        case _ => if (toNumber(v1) >= toNumber(v2)) true else false
      }
    }
  }
  /* Big-Step Interpreter with Dynamic Scoping */
  
  /*
   * Start by copying your code from Lab 2 here.
   */
  def eval(env: Env, e: Expr): Expr = {
    e match {
      /* Base Cases */
      case N(_) | B(_) | S(_) | Undefined | Function(_, _, _) => e
      case Var(x) => lookup(env, x)
      
      /* Inductive Cases */
      case ConstDecl(x, e1, e2) => {
        val v = eval(env, e1)
        val env2 = extend(env, x, v)
        eval(env2, e2)
      }
      case Print(e1) => println(pretty(eval(env, e1))); Undefined
      case Unary(uop, e1) =>
        val first = eval(env, e1)
        uop match {
          case Neg => N(-toNumber(first))
          case Not => B(!toBoolean(first))
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
          case Eq => (first, second) match {
            case (Function(_,_,_), _) => throw DynamicTypeError(e)
            case (_, Function(_,_,_)) => throw DynamicTypeError(e)
            case _ => B(if (first == second) true else false) // eval(env, e1) === eval(env, e2)
        }

          case Ne => (first, second) match {
            case (Function(_,_,_), _) => throw DynamicTypeError(e)
            case (_, Function(_,_,_)) => throw DynamicTypeError(e)
            case _ => B(if (first == second) false else true) // eval(env, e1) !== eval(env, e2)
          }
          case Lt => B(inequalityVal(Lt, first, second)) // eval(env, e1) < eval(env, e2)
          case Le => B(inequalityVal(Le, first, second)) // eval(env, e1) <= eval(env, e2)
          case Gt => B(inequalityVal(Gt, first, second)) // eval(env, e1) > eval(env, e2)
          case Ge => B(inequalityVal(Ge, first, second)) // eval(env, e1) >= eval(env, e2)
          case Seq => second // eval(env, e1), eval(env, e2)
        }
      case If(e1, e2, e3) =>
        if (toBoolean(eval(env, e1))) eval(env, e2) else eval(env, e3) // e1? then e2 else e3
      case Call(e1, arg) =>
        val v1 = eval(env, e1)
        v1 match {
        case Function(None, param, exprToEval) => {
          val solv: Expr = ConstDecl(param, arg, exprToEval)
          eval(env, solv)
        }
        case Function(Some(functName), param, exprToEval) => {
          val env2 = extend(env, functName, Function(None, param, exprToEval))
          val solv: Expr = ConstDecl(param, arg, exprToEval)
          eval(env2, solv)
        }
//        case Var(f) =>
//          lookup(env, f) match {
//          case Function(n, param, exprToEval) => eval(env, Call(Function(n, param, exprToEval), arg))
//          case _ => throw DynamicTypeError(e)
//        }
        case _ => throw DynamicTypeError(e)
      }
    }
  }
    

  /* Small-Step Interpreter with Static Scoping */

  def iterate(e0: Expr)(next: (Expr, Int) => Option[Expr]): Expr = {
    def loop(e: Expr, n: Int): Expr = next(e, n) match {
      case None => e
      case Some(f) => loop(f, n+1)
    }
    loop(e0, 0)
  }
  
  def substitute(e: Expr, v: Expr, x: String): Expr = { // Replace as instances of x with v in expression e.
    require(isValue(v))
    e match {
      case N(_) | B(_) | Undefined | S(_) => e
      case Print(e1) => Print(substitute(e1, v, x))
      case Unary(uop, e1) => Unary(uop, substitute(e1, v, x))
      case Binary(bop, e1, e2) => Binary(bop, substitute(e1, v ,x), substitute(e2, v ,x))
      case If(e1, e2, e3) => If(substitute(e1, v, x), substitute(e2, v, x), substitute(e3, v, x))
      case Call(e1, e2) => Call(substitute(e1, v, x), substitute(e2, v, x))
      case Var(y) => {
        if (y == x) v // If the sub value is the same as the variable name, then returns the expression to be put into that value.
        else e // else is not the correct variable so returns the unchanged expression
      }
      case Function(None, y, e1) => {
        if (x == y) e // Substitute variable is the same as the parameter, so returns the function because nothing is changing.
        else Function(None, y, substitute(e1, v, x)) // If they are not the same then calls sub again on the expression part of the function.
      }
      case Function(Some(y1), y2, e1) => {
        if (x == y1) e  // Substitute is the same as the name of the function. Return the expression because the function is substituting itself.
        else if (x == y2) e // Substitute is the same as the parameter of the function. Return the function because nothing is being replaced inside.
        else Function(Some(y1), y2, substitute(e1, v, x)) // If not, then call substitute again on the expression to become the parameter.
      }
      case ConstDecl(y, e1, e2) => {
        if (x == y) ConstDecl(y, substitute(e1, v, x), e2) // If the name of the ConstDecl = substitution name, only want to call sub on first expression because want variable to be redefined.
        else ConstDecl(y, substitute(e1, v, x), substitute(e2, v, x)) // If not the name, then call sub on both parts.
      }
    }
  }
    
  def step(e: Expr): Expr = {
    e match {
      /* Base Cases: Do Rules */
      case Print(v1) if isValue(v1) => println(pretty(v1)); Undefined
        // ****** Your cases here
      case Unary(uop, e1) if isValue(e1) =>
        uop match {
          case Neg => N(-toNumber(e1))
          case Not => B(!toBoolean(e1))
      }
      case Binary(And, e1, e2) if isValue(e1) => {
        if (toBoolean(e1)) e2
        else e1
      }
      case Binary(Or, e1, e2) if isValue(e1) => {
        if (toBoolean(e1)) e1
        else e2
      }
      case Binary(Seq, e1, e2) if isValue(e1) => e2
      case Binary(bop, e1, e2) if isValue(e1) && isValue(e2) => bop match {
          case Plus => (e1, e2) match {
            case (S(s1), _) => S(s1 + toStr(e2))
            case (_, S(s2)) => S(toStr(e1) + s2)
            case _ => N(toNumber(e1) + toNumber(e2))
          }
          case Minus => N(toNumber(e1) - toNumber(e2))
          case Times => N(toNumber(e1) * toNumber(e2))
          case Div => N(toNumber(e1) / toNumber(e2))
          case Eq => (e1, e2) match {
            case (Function(_,_,_),_) => throw new DynamicTypeError(e)
            case (_,Function(_,_,_)) => throw DynamicTypeError(e)
            case _ => B(if(e1 == e2) true else false)
          }
          case Ne => (e1, e2) match {
            case (Function(_,_,_), _) => throw DynamicTypeError(e)
            case (_, Function(_,_,_)) => throw DynamicTypeError(e)
            case _ => B(if(e1 != e2) true else false)
          }
          case Lt => B(inequalityVal(Lt, e1, e2))
          case Le => B(inequalityVal(Le, e1, e2))
          case Gt => B(inequalityVal(Gt, e1, e2))
          case Ge => B(inequalityVal(Ge, e1, e2))
        }
      case If(e1, e2, e3) if isValue(e1) => {
        if (toBoolean(e1)) e2
        else e3
      }
      case Call(v1, v2) if isValue(v1) && isValue(v2) => v1 match {
        case Function(None, x, e1) => substitute(e1, v2, x)
        case Function(Some(x1), x2, e1) => substitute(substitute(e1, v1, x1), v2, x2)
        case _ => throw DynamicTypeError(e)
      }
      case ConstDecl(x, e1, e2) if isValue(e1) => {
        substitute(e2, e1, x)
      }
      /* Inductive Cases: Search Rules */
      case Print(e1) => Print(step(e1))
      
        // ****** Your cases here
      case Unary(uop, e1) => Unary(uop, step(e1))
      case Binary(e1 @ (Eq|Ne), Function(_,_,_), v2) => throw new DynamicTypeError(e)
      case Binary(bop, v1, e2) if isValue(v1) => Binary(bop, v1, step(e2)) // If first is a value then second is not so calls step on second.
      case Binary(bop, e1, e2) => Binary(bop, step(e1), e2) // If first is not a value, then calls step on first. Also catches if neither is a value.
      case If(e1, e2, e3) => If(step(e1), e2, e3)
      case ConstDecl(x, e1, e2) => ConstDecl(x, step(e1), e2)
      case Call(e1 @ Function(_,_,_), e2) => Call (e1, step(e2))
      case Call(e1, e2) if isValue(e1) => throw new DynamicTypeError(e)
      case Call(e1, e2) => Call(step(e1), e2)
      /* Cases that should never match. Your cases above should ensure this. */
      case Var(_) => throw new AssertionError("Gremlins: internal error, not closed expression.")
      case N(_) | B(_) | Undefined | S(_) | Function(_, _, _) => throw new AssertionError("Gremlins: internal error, step should not be called on values.");
    }
  }


  /* External Interfaces */
  
  //this.debug = true // uncomment this if you want to print debugging information
  this.keepGoing = true // comment this out if you want to stop at first exception when processing a file

}

package jsy.student

import jsy.lab4.Lab4Like

object Lab4 extends jsy.util.JsyApplication with Lab4Like {
  import jsy.lab4.ast._
  import jsy.lab4.Parser

  /*
   * CSCI 3155: Lab 4
   * Adam Ten Hoeve
   *
   * Partner: Kyle Helmick
   * Collaborators: Dylan Fox, Nahar
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
   * '???' as needed to get something that compiles without error. The '???'
   * is a Scala expression that throws the exception scala.NotImplementedError.
   */

  /* Collections and Higher-Order Functions */

  /* Lists */

  def compressRec[A](l: List[A]): List[A] = l match {
    case Nil | _ :: Nil => l
    case h1 :: (t1 @ (h2 :: _)) => {
      if (h1 == h2) compressRec(t1)
      else h1 :: compressRec(t1)
    }
  }

  def compressFold[A](l: List[A]): List[A] = l.foldRight(Nil: List[A]){
    (h, acc) => acc match {
      case Nil => h :: Nil
      case t1 @ (h2 :: _) =>
        if (h == h2) t1
        else h :: t1
    }
  }

  def mapFirst[A](l: List[A])(f: A => Option[A]): List[A] = l match {
    case Nil => l
    case h :: t => f(h) match {
      case None => h :: mapFirst(t)(f)
      case Some(x) => x :: t
    }
  }

  /* Trees */

  def foldLeft[A](t: Tree)(z: A)(f: (A, Int) => A): A = {
    def loop(acc: A, t: Tree): A = t match {
      case Empty => acc
      case Node(l, d, r) => loop(f(loop(acc, l), d), r) //loops through left, performs function, then loops through right
    }
    loop(z: A, t)
  }

  // An example use of foldLeft
  def sum(t: Tree): Int = foldLeft(t)(0){ (acc, d) => acc + d }

  // Create a tree from a list. An example use of the
  // List.foldLeft method.
  def treeFromList(l: List[Int]): Tree =
    l.foldLeft(Empty: Tree){ (acc, i) => acc insert i }

  def strictlyOrdered(t: Tree): Boolean = {
    val (b, _) = foldLeft(t)(true, None: Option[Int]){ // We are making the third parameter for foldLeft
      (acc, num) => acc match {
        case (false, _) => (false, None)
        case (true, None) => (true, Some(num))
        case (true, Some(x)) => {
          if (x < num) (true, Some(num))
          else (false, None)
        }
      }
    }
    b
  }

  /* Type Inference */

  // While this helper function is completely given, this function is
  // worth studying to see how library methods are used.
  def hasFunctionTyp(t: Typ): Boolean = t match {
    case TFunction(_, _) => true
    case TObj(fields) if (fields exists { case (_, t) => hasFunctionTyp(t) }) => true
    case _ => false
  }

  def typeof(env: TEnv, e: Expr): Typ = {
    def err[T](tgot: Typ, e1: Expr): T = throw StaticTypeError(tgot, e1, e)

    e match {
      case Print(e1) => typeof(env, e1); TUndefined
      case N(_) => TNumber
      case B(_) => TBool
      case Undefined => TUndefined
      case S(_) => TString
      case Var(x) => lookup(env, x)
      case Decl(mode, x, e1, e2) => {
        val t1 = typeof(env, e1)
        val env2 = extend(env, x, t1)
        typeof(env2, e2)
      }
      case Unary(Neg, e1) =>
        typeof(env, e1) match {
          case TNumber => TNumber
          case tgot => err(tgot, e1)
        }
      case Unary(Not, e1) =>
        val t1 = typeof(env, e1)
        t1 match {
          case TBool => TBool
          case tgot => err(tgot, e1)
        }
      case Binary(Plus, e1, e2) => {
        val t1 = typeof(env, e1)
        val t2 = typeof(env, e2)
        (t1, t2) match {
          case (TNumber, TNumber) => TNumber
          case (TString, TString) => TString
          case (TNumber, tgot) => err(tgot, e2)
          case (TString, tgot) => err(tgot, e2)
          case (tgot, _) => err(tgot, e1)
        }
      }
      case Binary(Minus | Times | Div, e1, e2) =>
        val t1 = typeof(env, e1)
        val t2 = typeof(env, e2)
        (t1, t2) match {
          case (TNumber, TNumber) => TNumber
          case (TNumber, tgot) => err(tgot, e2)
          case (tgot, _) => err(tgot, e1)
        }
      //      case Binary(Eq | Ne, e1, e2) => {
      //        val t1 = typeof(env, e1)
      //        val t2 = typeof(env, e2)
      //        if (t1 != t2) err(t2, e2)
      //        else {
      //          (t1, t2) match {
      //            case (tgot @ TFunction(_, _), _) => err(tgot, e1)
      //            case (_, tgot @ TFunction(_, _)) => err(tgot, e2)
      //          }
      //          TBool
      //        }
      //      }
      case Binary(Eq|Ne, e1, e2) => (typeof(env, e1), typeof(env, e2)) match {
        case (TFunction(a, b), _) => err(TFunction(a, b), e1) // Remember the correct usage of TFunction is with it's parameters
        case (_, TFunction(a, b)) => err(TFunction(a, b), e2) // Remember the correct usage of TFunction is with it's parameters
        case (a, b) => if (a == b) TBool else err(typeof(env, e2), e2)
      }
      case Binary(Lt|Le|Gt|Ge, e1, e2) => {
        val t1 = typeof(env, e1)
        val t2 = typeof(env, e2)
        (t1, t2) match {
          case (TNumber, TNumber) => TBool
          case (TString, TString) => TBool
          case (TNumber, tgot) => err(tgot, e2)
          case (TString, tgot) => err(tgot, e2)
          case (tgot, _) => err(tgot, e1)
        }
      }
      case Binary(And|Or, e1, e2) =>{
        val t1 = typeof(env, e1)
        val t2 = typeof(env, e2)
        (t1, t2) match {
          case (TBool, TBool) => TBool
          case (TBool, tgot) => err(tgot, e2)
          case (tgot, _) => err(tgot, e1)
        }
      }
      case Binary(Seq, e1, e2) => {
        val t1 = typeof(env, e1)
        typeof(env, e2)
      }
      case If(e1, e2, e3) => typeof(env, e1) match {
        case TBool => {
          val t2 = typeof(env, e2)
          val t3 = typeof(env, e3)
          if (t2 == t3) t2           // If the types are the same, then return their type
          else err(t2, e2)           // If the types don't match then throw error
        }
        case t1 => err(t1, e1)   // e1 isn't TBool so error
      }
      case Function(p, params, tann, e1) => {
        // Bind to env1 an environment that extends env with an appropriate binding if
        // the function is potentially recursive.
        val env1 = (p, tann) match {
          /***** Add cases here *****/
          case (None, _) => env
          //case (None, Some(t)) => if (typeof(env, e1) == t) env else err(typeof(env, e1), e1)
          case (Some(name), Some(t)) => {
            val tau_prime = TFunction(params, t)
            extend(env, name, tau_prime)
          }
          case _ => err(TUndefined, e1)
        }
        // Bind to env2 an environment that extends env1 with bindings for params.
        val env2 = params.foldLeft(env1){
          (acc, d) => extend(acc, d._1, d._2.t) // d is tuple of (string, MTyp)
        }
        // Infer the type of the function body
        val t1 = typeof(env2, e1)
        // Check with the possibly annotated return type
        tann match {
          case None => TFunction(params, t1)
          case Some(x) => if (t1 == x) TFunction(params, t1) else err(t1, e1)
        }
      }
      case Call(e1, args) => typeof(env, e1) match {
        case TFunction(params, tret) if (params.length == args.length) =>
          (params zip args).foreach {
            case ((_, mtyp), exp) => { // looks like ((string, mtyp), Expr) -> ((string, (m, t)), Expr)
              if (mtyp.t != typeof(env, exp)) throw err(typeof(env, exp), exp) // If any does not match then throws error
            }
          };
          tret
        case tgot => err(tgot, e1)
      }
      case Obj(fields) => TObj(fields.map(field => (field._1, typeof(env, field._2))))
      case GetField(e1, f) => typeof(env, e1) match { // Want to match the type of e1
        case TObj(field) => field.get(f) match { //
          case Some(t) => t
          case _ => err(typeof(env, e1), e1)
        }
        case _ => err(typeof(env, e1), e1) // throw error if not an object
      }
    }
  }


  /* Small-Step Interpreter */

  /*
   * Helper function that implements the semantics of inequality
   * operators Lt, Le, Gt, and Ge on values.
   *
   * We suggest a refactoring of code from Lab 2 to be able to
   * use this helper function in eval and step.
   *
   * This should the same code as from Lab 3.
   */
  def inequalityVal(bop: Bop, v1: Expr, v2: Expr): Boolean = {
    require(isValue(v1), s"inequalityVal: v1 ${v1} is not a value")
    require(isValue(v2), s"inequalityVal: v2 ${v2} is not a value")
    require(bop == Lt || bop == Le || bop == Gt || bop == Ge)
    (v1, v2) match {
      case (S(s1), S(s2)) => {
        bop match {
          case Lt => if (s1 < s2) true else false
          case Le => if (s1 <= s2) true else false
          case Gt => if (s1 > s2) true else false
          case Ge => if (s1 >= s2) true else false
        }
      }
      case (N(n1), N(n2)) => {
        bop match{
          case Lt => if (n1 < n2) true else false
          case Le => if (n1 <= n2) true else false
          case Gt => if (n1 > n2) true else false
          case Ge => if (n1 >= n2) true else false
        }
      }
    }
  }

  /* This should be the same code as from Lab 3 */
  def iterate(e0: Expr)(next: (Expr, Int) => Option[Expr]): Expr = {
    def loop(e: Expr, n: Int): Expr = next(e, n) match {
      case None => e
      case Some(f) => loop(f, n+1)
    }
    loop(e0, 0)
  }

  /* Capture-avoiding substitution in e replacing variables x with esub. */
  def substitute(e: Expr, esub: Expr, x: String): Expr = { // Replace all instances of string x with expression esub within e
    def subst(e: Expr): Expr = e match {
      case N(_) | B(_) | Undefined | S(_) => e
      case Print(e1) => Print(substitute(e1, esub, x))
      /***** Cases from Lab 3 */
      case Unary(uop, e1) => Unary(uop, substitute(e1, esub, x))
      case Binary(bop, e1, e2) => Binary(bop, substitute(e1, esub, x), substitute(e2, esub, x))
      case If(e1, e2, e3) => If(substitute(e1, esub, x), substitute(e2, esub, x), substitute(e3, esub, x))
      case Var(y) => {
        if (y == x) esub // If the sub name is the same as the variable name, then returns the expression to be substituted in
        else e // is not the correct variable so returns the expression without changing anything
      }
      case Decl(mode, y, e1, e2) => {
        if (x == y) Decl(mode, y, substitute(e1, esub, x), e2) // If names are the same, only call sub on first because we want variable to be redefined
        else Decl(mode, y, substitute(e1, esub, x), substitute(e2, esub, x)) // If not the same, call sub on both parts
      }
      /***** Cases needing adapting from Lab 3 */
      case Function(p, params, tann, e1) => {
        if (params.exists((tt :(String,MTyp)) => tt._1 == x) || p == Some(x)) e // If the name of the function or any of the parameters are equal to the string, then just return the function
        else Function(p, params, tann, substitute(e1, esub, x)) // otherwise not the same so check the body for substituted variable
      }
      case Call(e1, args) => Call(substitute(e1, esub, x), args.map(v => substitute(v, esub, x)))
      /***** New cases for Lab 4 */
      case Obj(fields) => Obj(fields.mapValues(e => substitute(e, esub, x)))
      case GetField(e1, f) => GetField(substitute(e1, esub, x), f)
    }

    val fvs = freeVars(esub)
    def fresh(x: String): String = if (fvs.contains(x)) fresh(x + "$") else x
    subst(rename(e)(fresh))
    //subst(e)
  }

  /* Rename bound variables in e */
  def rename(e: Expr)(fresh: String => String): Expr = {
    def ren(env: Map[String,String], e: Expr): Expr = {
      e match {
        case N(_) | B(_) | Undefined | S(_) => e
        case Print(e1) => Print(ren(env, e1))

        case Unary(uop, e1) => Unary(uop, ren(env, e1))
        case Binary(bop, e1, e2) => Binary(bop, ren(env, e1), ren(env, e2))
        case If(e1, e2, e3) => If(ren(env, e1), ren(env, e2), ren(env, e3))

        case Var(y) => {
          if (env.contains(y)) Var(env(y)) // Bound earlier in environment
          else Var(y) // Free variable, unset
        }
        case Decl(mode, y, e1, e2) =>
          val yp = fresh(y)
          Decl(mode, yp, ren(env, e1), ren(env + (y -> yp), e2))

        case Function(p, params, retty, e1) => {
          val (pp, envp): (Option[String], Map[String,String]) = p match {
            case None => (None, env)
            case Some(name) => {
              val freshname = fresh(name)
              (Some(freshname), extend(env, name, freshname))
            }
          }
          val (paramsp, envpp) = params.foldRight( (Nil: List[(String,MTyp)], envp) ) {
            case ((name, mtyp), acc) => {
              val freshparam = fresh(name)
              ((freshparam, mtyp) :: acc._1, extend(acc._2, name, freshparam))
            }
          }
          Function(pp, paramsp, retty, ren(envpp, e1))
        }

        case Call(e1, args) => {
          val renamedArgs = args.foldRight(Nil: List[(Expr)]) {
            case (d, acc) => ren(env, d) :: acc
          }
          Call(ren(env, e1), renamedArgs)
        }
        case Obj(fields) => Obj(fields.map(f => (fresh(f._1), ren(extend(env, f._1, fresh(f._1)), f._2))))
        case GetField(e1, f) => GetField(e1, f)
      }
    }
    ren(empty, e)
  }

  /* Check whether or not an expression is reduced enough to be applied given a mode. */
  def isRedex(mode: Mode, e: Expr): Boolean = mode match {
    case MConst => if (isValue(e)) false else true
    case MName => false
  }

  def step(e: Expr): Expr = {
    require(!isValue(e), s"step: e ${e} to step is a value")
    e match {
      /* Base Cases: Do Rules */
      case Print(v1) if isValue(v1) => println(pretty(v1)); Undefined
      /***** Cases needing adapting from Lab 3. */
      case Unary(Neg, v1) if isValue(v1) => v1 match {
        case N(n) => N(-n)
      }
      case Unary(Not, v1) if isValue(v1) => v1 match {
        case B(b) => B(!b)
      }
      case Binary(Seq, v1, e2) if isValue(v1) => e2
      case Binary(And, v1, e2) if isValue(v1) => v1 match {
        case B(true) => e2
        case B(false) => B(false)
      }
      case Binary(Or, v1, e2) if isValue(v1) => v1 match {
        case B(true) => B(true)
        case B(false) => e2
      }
      case Binary(bop, v1, v2) if isValue(v1) && isValue(v2) =>
        (v1, v2) match {
          case (S(s1), S(s2)) => bop match {
            case Plus => S(s1 + s2)
            case b @ (Lt | Le | Gt | Ge) => B(inequalityVal(b, v1, v2))
            case Eq => B(if(v1 == v2) true else false)
            case Ne => B(if(v1 != v2) true else false)
          }
          case (N(n1), N(n2)) => bop match {
            case Plus => N(n1 + n2)
            case Minus => N(n1 - n2)
            case Times => N(n1 * n2)
            case Div => N(n1 / n2)
            case b @ (Lt | Le | Gt | Ge) => B(inequalityVal(b, v1, v2))
            case Eq => B(if(v1 == v2) true else false)
            case Ne => B(if(v1 != v2) true else false)
          }
          case (_, _) => bop match {
            case Eq => B(if(v1 == v2) true else false)
            case Ne => B(if(v1 != v2) true else false)
          }
        }
      case If(v1, e2, e3) if isValue(v1) => v1 match {
        case B(true) => e2
        case B(false) => e3
      }
      case Decl(mode, x, e1, e2) if !isRedex(mode, e1) => substitute(e2, e1, x)
      /***** More cases here */
      case Call(v1, args) if isValue(v1) =>
        v1 match {
          case Function(p, params, _, e1) => {
            val pazip = params zip args
            if (pazip.forall(f => !isRedex(f._1._2.m, f._2))) {
              val e1p = pazip.foldRight(e1) {
                (tuple, acc) =>  substitute(acc, tuple._2 ,tuple._1._1)
              }
              p match {
                case None => e1p
                case Some(x1) => substitute(e1p, v1, x1)
              }
            }
            else {
              val pazipp = mapFirst(pazip) {
                case (tuple, arg) if isRedex(tuple._2.m, arg) => Some(tuple, step(arg))
                case _ => None
              }
              val unzip = pazipp.unzip
              Call(v1, unzip._2)
            }
          }
          case _ => throw StuckError(e)
        }
      /***** New cases for Lab 4. */
      case GetField(e1, f) if isValue(e1) => e1 match {
        case Obj(fields) => fields.get(f) match {
          case Some(x) => x
          case None => Undefined
        }
      }


      /* Inductive Cases: Search Rules */
      case Print(e1) => Print(step(e1))
      /***** Cases from Lab 3. */
      case Unary(uop, e1) => Unary(uop, step(e1))
      case Binary(bop, v1, e2) if isValue(v1) => Binary(bop, v1, step(e2))
      case Binary(bop, e1, e2) => Binary(bop, step(e1), e2)
      case If(e1, e2, e3) => If(step(e1), e2, e3)
      case Decl(mode, x, e1, e2) if isRedex(mode, e1) => Decl(mode, x, step(e1), e2)
      /***** More cases here */
      /***** Cases needing adapting from Lab 3 */
      case Call(e1, args) => Call(step(e1), args)
      /***** New cases for Lab 4. */
      case Obj(field) => {
        val newList = field.toList
        val steppedList = mapFirst(newList){
          case (f, value) if !isValue(value) => Some((f, step(value)))
          case _ => None
        }
        Obj(steppedList.toMap)
      }
      case GetField(e1, f) => GetField(step(e1), f)
      /* Everything else is a stuck error. Should not happen if e is well-typed.
       *
       * Tip: you might want to first develop by comment out the following line to see which
       * cases you have missing. You then uncomment this line when you are sure all the cases
       * that you have left the ones that should be stuck.
       */
      case _ => throw StuckError(e)
    }
  }


  /* External Interfaces */

  //this.debug = true // uncomment this if you want to print debugging information
  this.keepGoing = true // comment this out if you want to stop at first exception when processing a file
}

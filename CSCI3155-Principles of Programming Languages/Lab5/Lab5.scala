package jsy.student

import jsy.lab5.Lab5Like

object Lab5 extends jsy.util.JsyApplication with Lab5Like {
  import jsy.lab5.ast._
  import jsy.util.DoWith
  import jsy.util.DoWith._

  /*
   * CSCI 3155: Lab 5
   * Adam Ten Hoeve
   *
   * Partner: <Your Partner's Name>
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
   * '???' as needed to get something that compiles without error. The '???'
   * is a Scala expression that throws the exception scala.NotImplementedError.
   */

  /*** Exercise with DoWith ***/

  def rename[W](env: Map[String,String], e: Expr)(fresh: String => DoWith[W,String]): DoWith[W,Expr] = {
    def ren(env: Map[String,String], e: Expr): DoWith[W,Expr] = e match {
      case N(_) | B(_) | Undefined | S(_) | Null | A(_) => doreturn(e)
      case Print(e1) => ren(env,e1) map { e1p => Print(e1p) }

      case Unary(uop, e1) => {
        val dw = ren(env, e1)
        dw.map(expr => Unary(uop, expr))
      }
      case Binary(bop, e1, e2) => {
        val dw1 = ren(env, e1)
        val dw2 = ren(env, e2)
        dw1.flatMap(e1p => dw2.map(e2p => Binary(bop, e1p, e2p)))
      }
      case If(e1, e2, e3) => {
        val dw1 = ren(env, e1)
        val dw2 = ren(env, e2)
        val dw3 = ren(env, e3)
        dw1.flatMap(e1p => dw2.flatMap(e2p => dw3.map(e3p => If(e1p, e2p, e3p))))
      }

      case Var(x) => doreturn(Var(env.getOrElse(x, x)))

      case Decl(m, x, e1, e2) => fresh(x) flatMap {
        xp:String => {
          val dw1 = ren(env, e1)
          val dw2 = ren(env + (x -> xp), e2)
          dw1.flatMap(e1p => dw2.map(e2p => Decl(m, xp, e1p, e2p)))
        }
      }

      case Function(p, params, retty, e1) => {
        val w: DoWith[W,(Option[String], Map[String,String])] = p match {
          case None => doreturn((None, env))
          case Some(x) => {
            val dw = fresh(x)
            dw.map(newx => (Some(newx), env + (x -> newx)))
          }
        }
        w flatMap { case (pp, envp) =>
          params.foldRight[DoWith[W,(List[(String,MTyp)],Map[String,String])]]( doreturn((Nil, envp)) ) {
            case ((x,mty), acc) => acc flatMap {
              accp => {
                val dw = fresh(x)
                dw.map(newx => ((newx, mty) :: accp._1, accp._2 + (x -> newx)))
              }
            }
          } flatMap { newparams => {
              val dw = ren(newparams._2, e1)
              dw.map( newBody => Function(pp, newparams._1, retty, newBody))
            }
          }
        }
      }

      case Call(e1, args) => {
        val dw = ren(env, e1)
        dw.flatMap(expr => mapWith(args)(arg => ren(env, arg)).map(newArgs => Call(expr, newArgs)))
      }

      case Obj(fields) => mapWith(fields){
        case (name, e1) => ren(env, e1).map{e1p => (name, e1p)}
      }.map{f => Obj(f)}
      case GetField(e1, f) => ren(env, e1).map(e1p => GetField(e1p, f))

      case Assign(e1, e2) => {
        val dw1 = ren(env, e1)
        val dw2 = ren(env, e2)
        dw1.flatMap(e1p => dw2.map(e2p => Assign(e1p, e2p)))
      }

      /* Should not match: should have been removed */
      case InterfaceDecl(_, _, _) => throw new IllegalArgumentException("Gremlins: Encountered unexpected expression %s.".format(e))
    }
    ren(env, e)
  }

  def myuniquify(e: Expr): Expr = {
    val fresh: String => DoWith[Int,String] = { _ =>
      doget[Int].flatMap((i:Int) => {
        val RenamedString = "x" + i.toString()
        doput(i+1).map( _ => RenamedString)
      })
    }
    val (_, r) = rename(empty, e)(fresh)(0)
    r
  }

  /*** Helper: mapFirst to DoWith ***/

  // List map with an operator returning a DoWith
  def mapWith[W,A,B](l: List[A])(f: A => DoWith[W,B]): DoWith[W,List[B]] = {
    l.foldRight[DoWith[W,List[B]]]( doreturn(Nil) ) { // Traverse over hole list and performing f on each element to add to new list
      case (a, dwbs) => dwbs.flatMap((bs:List[B]) => f(a).map((b) => b::bs)) // For each element a, adds the element to the list within the DoWith
    }
  }

  // Map map with an operator returning a DoWith
  def mapWith[W,A,B,C,D](m: Map[A,B])(f: ((A,B)) => DoWith[W,(C,D)]): DoWith[W,Map[C,D]] = {
    m.foldRight[DoWith[W,Map[C,D]]]( doreturn(Map()) ) {
      case (a:(A,B), acc) => acc.flatMap((m:Map[C,D]) => f(a).map((tuple:(C,D)) => m + (tuple._1 -> tuple._2)))
    }
  }

  // Just like mapFirst from Lab 4 but uses a callback f that returns a DoWith in the Some case.
  def mapFirstWith[W,A](l: List[A])(f: A => Option[DoWith[W,A]]): DoWith[W,List[A]] = l match {
    case Nil => doreturn(l)
    case h :: t => f(h) match {
      case None => mapFirstWith(t)(f).map((t:List[A]) => h :: t)
      case Some(x) => x.map((y:A) => y :: t)
    }
  }

  // There are better ways to deal with the combination of data structures like List, Map, and
  // DoWith, but we won't tackle that in this assignment.

  /*** Casting ***/

  def castOk(t1: Typ, t2: Typ): Boolean = (t1, t2) match {
      /***** Make sure to replace the case _ => ???. */
    case (TNull, TObj(_)) => true
    case (_, _) if t1 == t2 => true
    case (TObj(field1), TObj(field2)) => field1.forall{
      case (_, value) if value == Null => true
      case (key, value) => field2.get(key) match {
        case None => true
        case Some(value2) => value == value2
      }
    }

      /***** Cases for the extra credit. Do not attempt until the rest of the assignment is complete. */
    case (TInterface(tvar, t1p), _) => ???
    case (_, TInterface(tvar, t2p)) => ???
      /***** Otherwise, false. */
    case _ => false
  }

  /*** Type Inference ***/

  // A helper function to check whether a jsy type has a function type in it.
  // While this is completely given, this function is worth studying to see
  // how library functions are used.
  def hasFunctionTyp(t: Typ): Boolean = t match {
    case TFunction(_, _) => true
    case TObj(fields) if (fields exists { case (_, t) => hasFunctionTyp(t) }) => true
    case _ => false
  }

  def isBindex(m: Mode, e: Expr): Boolean = m match {
    case MConst | MName | MVar => true
    case MRef => isLExpr(e)
  }

  def typeof(env: TEnv, e: Expr): Typ = {
    def err[T](tgot: Typ, e1: Expr): T = throw StaticTypeError(tgot, e1, e)
    def t(e: Expr): Typ = typeof(env, e)

    e match {
      case Print(e1) => typeof(env, e1); TUndefined
      case N(_) => TNumber
      case B(_) => TBool
      case Undefined => TUndefined
      case S(_) => TString
      case Var(x) => lookup(env, x).t
      case Unary(Neg, e1) => typeof(env, e1) match {
        case TNumber => TNumber
        case tgot => err(tgot, e1)
      }
        /***** Cases directly from Lab 4. We will minimize the test of these cases in Lab 5. */
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
      case Obj(fields) => TObj(fields.map(field => (field._1, typeof(env, field._2))))
      case GetField(e1, f) => typeof(env, e1) match { // Want to match the type of e1
        case TObj(field) => field.get(f) match { //
          case Some(t) => t
          case _ => err(typeof(env, e1), e1)
        }
        case _ => err(typeof(env, e1), e1) // throw error if not an object
      }

        /***** Cases from Lab 4 that need a small amount of adapting. */
      case Decl(m, x, e1, e2) =>
        val t1 = typeof(env, e1)
        if (isBindex(m, e1)) {
          val env2 = env + (x -> MTyp(m, t1))
          typeof(env2, e2)
        }
        else err(typeof(env, e1), e1)

      case Function(p, params, tann, e1) => {
        // Bind to env1 an environment that extends env with an appropriate binding if
        // the function is potentially recursive.
        val env1 = (p, tann) match {
          case (Some(f), Some(tret)) =>
            val tprime = TFunction(params, tret)
            env + (f -> MTyp(MConst, tprime))
          case (None, _) => env
          case _ => err(TUndefined, e1)
        }
        // Bind to env2 an environment that extends env1 with bindings for params.
        val env2 = (params :\ env1)((t, acc) => acc + (t._1 -> t._2)) // fold on env1 as the base value to update with all parameter names

        // Infer the type of the function body
        val t1 = typeof(env2, e1)

        // Match on whether the return type is specified.
        tann match {
          case None => TFunction(params, t1)
          case Some(tret) => if (tret == t1) TFunction(params, t1) else err(t1, e1)
        }
      }
      case Call(e1, args) => typeof(env, e1) match {
        case TFunction(params, tret) if (params.length == args.length) =>
          (params, args).zipped.foreach {
            case ((name, mtyp), expr) => if (mtyp.t != typeof(env, expr) || !isBindex(mtyp.m, expr)) throw err(typeof(env, expr), expr)
          }
          tret
        case tgot => err(tgot, e1)
      }

        /***** New cases for Lab 5. ***/
//      case Assign(Var(x), e1) =>
//        env(x) match {
//          case MTyp(MVar|MRef, tx) =>
//            val t1 = t(e1)
//            if(tx == t1) tx
//            else err(t1, e1)
//          case tgot => err(tgot.t, e1)
//        }
      case Assign(Var(x), e1) =>
        val mtyp = env(x)
        mtyp match {
          case MTyp(MVar | MRef, varType) => if (typeof(env, e1) == varType) varType else err(typeof(env, e1), e1)
          case mgot => err(mgot.t, e1)
        }

      case Assign(GetField(e1, f), e2) => {
        val t2 = typeof(env, e2)
        typeof(env, e1) match {
          case TObj(field) => field.get(f) match {
            case Some(t1) if t1 == t2 => t1
            case _ => err(t2, e2)
          }
          case tgot => err(tgot, e1)
        }
      }

      case Assign(_, _) => err(TUndefined, e)

      case Null => TNull

      case Unary(Cast(t), e1) => typeof(env, e1) match {
        case tgot if castOk(tgot, t) => t
        case tgot => err(tgot, e1)
      }

      /* Should not match: non-source expressions or should have been removed */
      case A(_) | Unary(Deref, _) | InterfaceDecl(_, _, _) => throw new IllegalArgumentException("Gremlins: Encountered unexpected expression %s.".format(e))
    }
  }

  /*** Small-Step Interpreter ***/

  /*
   * Helper function that implements the semantics of inequality
   * operators Lt, Le, Gt, and Ge on values.
   *
   * We suggest a refactoring of code from Lab 2 to be able to
   * use this helper function in eval and step.
   *
   * This should the same code as from Lab 3 and Lab 4.
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

  /* Capture-avoiding substitution in e replacing variables x with esub. */
  def substitute(e: Expr, esub: Expr, x: String): Expr = { // Substitute all instance of x with esub in expression e
    def subst(e: Expr): Expr = e match {
      case N(_) | B(_) | Undefined | S(_) | Null | A(_) => e
      case Print(e1) => Print(subst(e1))
        /***** Cases from Lab 3 */
      case Unary(uop, e1) => Unary(uop, subst(e1))
      case Binary(bop, e1, e2) => Binary(bop, subst(e1), subst(e2))
      case If(e1, e2, e3) => If(subst(e1), subst(e2), subst(e3))
      case Var(y) => {
        if (x == y) esub // If the sub name is the same as the variable name, then returns the expression to be substituted in
        else e // // Is not the correct variable so returns the expression without changing anything
      }
        /***** Cases need a small adaption from Lab 3 */
      case Decl(mut, y, e1, e2) => Decl(mut, y, subst(e1), if (x == y) e2 else subst(e2))
        /***** Cases needing adapting from Lab 4 */
      case Function(p, paramse, retty, e1) =>
        if (paramse.exists((tt :(String,MTyp)) => tt._1 == x) || p == Some(x)) e // If x is any parameter or the name of the function, then return the expression
        else Function(p, paramse, retty, subst(e1)) // Else call subst on body
        /***** Cases directly from Lab 4 */
      case Call(e1, args) => Call(subst(e1), args.map(arg => subst(arg)))
      case Obj(fields) => Obj(fields.mapValues(expr => subst(expr)))
      case GetField(e1, f) => GetField(subst(e1), f)
        /***** New case for Lab 5 */
      case Assign(e1, e2) => Assign(subst(e1), subst(e2))

      /* Should not match: should have been removed */
      case InterfaceDecl(_, _, _) => throw new IllegalArgumentException("Gremlins: Encountered unexpected expression %s.".format(e))
    }

    def myrename(e: Expr): Expr = {
      val fvs = freeVars(esub)
      def fresh(x: String): String = if (fvs contains x) fresh(x + "$") else x
      rename[Unit](e)(){ x => doreturn(fresh(x)) }
    }
    subst(myrename(e))
  }

  /* Check whether or not an expression is reduced enough to be applied given a mode. */
  def isRedex(mode: Mode, e: Expr): Boolean = mode match {
    case m @ (MConst | MVar) => if (isValue(e)) false else true
    case MName => false
    case MRef => if (isLValue(e)) false else true
  }

  def getBinding(mode: Mode, e: Expr): DoWith[Mem,Expr] = {
    require(!isRedex(mode,e), s"expression ${e} must not reducible under mode ${mode}")
    mode match {
      case MConst if isValue(e) => doreturn(e)
      case MName => doreturn(e)
      case MRef if isLValue(e) => doreturn(e)
      case MVar if isValue(e) => memalloc(e).map((address) => Unary(Deref, address))
    }
  }

  /* A small-step transition. */
  def step(e: Expr): DoWith[Mem, Expr] = {
    require(!isValue(e), "stepping on a value: %s".format(e))
    e match {
      /* Base Cases: Do Rules */
      case Print(v1) if isValue(v1) => doget map { m => println(pretty(m, v1)); Undefined }
        /***** Cases needing adapting from Lab 3. */
      case Unary(Neg, N(n)) => doreturn(N(-n))
      case Unary(Not, B(b)) => doreturn(B(!b))
      case Binary(Seq, v1, e2) if isValue(v1) => doreturn(e2)
      case Binary(And, v1, e2) if isValue(v1) => v1 match {
        case B(true) => doreturn(e2)
        case B(false) => doreturn(B(false))
      }
      case Binary(Or, v1, e2) if isValue(v1) => v1 match {
        case B(true) => doreturn(B(true))
        case B(false) => doreturn(e2)
      }
      case Binary(bop, v1, v2) if (isValue(v1) && isValue(v2)) =>
        (v1, v2) match {
          case (S(s1), S(s2)) => bop match {
            case Plus => doreturn(S(s1 + s2))
            case b@(Lt | Le | Gt | Ge) => doreturn(B(inequalityVal(b, v1, v2)))
            case Eq => doreturn(B(if (s1 == s2) true else false))
            case Ne => doreturn(B(if (s1 != s2) true else false))
          }
          case (N(n1), N(n2)) => bop match {
            case Plus => doreturn(N(n1 + n2))
            case Minus => doreturn(N(n1 - n2))
            case Times => doreturn(N(n1 * n2))
            case Div => doreturn(N(n1 / n2))
            case b@(Lt | Le | Gt | Ge) => doreturn(B(inequalityVal(b, v1, v2)))
            case Eq => doreturn(B(if (n1 == n2) true else false))
            case Ne => doreturn(B(if (n1 != n2) true else false))
          }
          case (_, _) => bop match {
            case Eq => doreturn(B(if (v1 == v2) true else false))
            case Ne => doreturn(B(if (v1 != v2) true else false))
          }
        }
      case If(v1, e2, e3) if isValue(v1) => v1 match {
        case B(true) => doreturn(e2)
        case B(false) => doreturn(e3)
        case _ => throw StuckError(e)
      }

        /***** More cases here */
        /***** Cases needing adapting from Lab 4. */
      case Obj(fields) if (fields forall { case (_, vi) => isValue(vi)}) => {
        memalloc(Obj(fields))
      }


      case GetField(a @ A(_), f) =>
        doget map {
          mem => mem.get(a) match {
            case Some(Obj(field)) => field.get(f) match {
              case Some(x) => x
              case None => Undefined
            }
            case _ => throw StuckError(e)
          }
        }

      // DoDecl - const
      case Decl(MConst, x, v1, e2) if isValue(v1) =>
        doreturn(substitute(e2, v1, x))

      // DoDecl - var
      case Decl(MVar, x, v1, e2) if isValue(v1) =>
        memalloc(v1).map( a => substitute(e2, Unary(Deref, a), x) )

        /***** New cases for Lab 5. */
      case Unary(Deref, a @ A(_)) =>
        doget map {mem => mem.get(a) match {
          case None => throw StuckError(e)
          case Some(x) => x
        }}

      case Assign(Unary(Deref, a @ A(_)), v) if isValue(v) =>
        domodify[Mem] { mem => mem.+(a, v) } map { _ => v }

      case Assign(GetField(a @ A(_), f), v) if isValue(v) => {
        domodify[Mem] { mem =>
          mem.get(a) match {
            case Some(Obj(field)) => mem.+(a, Obj(field.updated(f, v)))
            case None => throw StuckError(e)
          }
        }.map(_ => v)
      }

      case Call(v1, args) if isValue(v1) => v1 match {
        case Function(p, params, _, body) => {
          val pazip = params zip args
          if (pazip.forall({case (arg, expr) => !isRedex(arg._2.m, expr)})) { // Enters if statement if none of the arguments are reducable
            // (arg, expr) : ((String, (MTyp, Typ)), Expr)
            val dwbody = pazip.foldRight(doreturn(body) : DoWith[Mem, Expr]) {
              case (tuple, acc) => acc.flatMap(expr => tuple._1._2.m match{ // tuple: ((String, MTyp), Expr)
                case MConst|MName|MRef => doreturn(substitute(expr, tuple._2, tuple._1._1))
                case MVar => memalloc(tuple._2).map(address => substitute(expr, Unary(Deref, address), tuple._1._1))
              })
            }
            p match { // Check if the function has a name
              case None => dwbody
              case Some(x) => dwbody.map(ep => substitute(ep, v1, x))
            }
          }
          else {
            val pazipp = mapFirstWith(pazip){
              case (tuple, arg) if (isRedex(tuple._2.m, arg)) => Some(step(arg).map((tuple, _)))
              case _ => None
            }
            pazipp.map(m => m unzip match {
              case (_, args) => Call(v1, args)
            })
          }
        }
        case _ => throw StuckError(e)
      }

      /* Base Cases: Error Rules */
        /***** Replace the following case with a case to throw NullDeferenceError.  */

      case Unary(Cast(TObj(_)), Null) => doreturn(Null)
      case Unary(Cast(TObj(fieldsTypes)), address @ A(_)) => {
        doget[Mem].map{
          mem => mem.get(address) match {
            case Some(Obj(fields)) => {
              if (fieldsTypes.forall({ case (name, typ) => fields.contains(name)})) address
              else throw DynamicTypeError(e)
            }
            case Some(_) => throw StuckError(e)
            case _ => throw DynamicTypeError(e)
          }
        }
      }
      case GetField(Null, _) => throw NullDereferenceError(e)
      case Assign(GetField(Null, _), _) => throw NullDereferenceError(e)

      case Unary(Cast(tau), v) if isValue(v) && v != Null => doreturn(v)

      /* Inductive Cases: Search Rules */
        /***** Cases needing adapting from Lab 3. Make sure to replace the case _ => ???. */
      case Print(e1) => step(e1) map { e1p => Print(e1p) }
      case Unary(uop, e1) => step(e1).map{e1p => Unary(uop, e1p)}
      case Binary(bop, v1, e2) if isValue(v1) => step(e2).map{e2p => Binary(bop, v1, e2p)}
      case Binary(bop, e1, e2) => step(e1).map{e1p => Binary(bop, e1p, e2)}
      case If(e1, e2, e3) => step(e1).map{e1p => If(e1p, e2, e3)}
        /***** Cases needing adapting from Lab 4 */
      case GetField(e1, f) => step(e1).map{e1p => GetField(e1p, f)}
      case Obj(fields) => {
        fields.find({case (f, value) => !isValue(value)}) match {
          case Some((x, e1)) => step(e1).map(e1p => Obj(fields + (x -> e1p)))
          case None => throw StuckError(e)
        }
      }

      case Decl(mode, x, e1, e2) => step(e1).map{e1p => Decl(mode, x, e1p, e2)}

      case Call(e1, args) => step(e1).map{e1p => Call(e1p, args)}

        /***** New cases for Lab 5.  */
      case Assign(e1, e2) if (!isLValue(e1)) => step(e1).map{e1p => Assign(e1p, e2)}
      case Assign(e1, e2) => step(e2).map{e2p => Assign(e1, e2p)}

      /* Everything else is a stuck error. */
      case _ => throw StuckError(e)
    }
  }

  /*** Extra Credit: Lowering: Remove Interface Declarations ***/

  def lower(e: Expr): Expr =
    /* Do nothing by default. Change to attempt extra credit. */
    e

  /*** External Interfaces ***/

  //this.debug = true // comment this out or set to false if you don't want print debugging information
  this.maxSteps = Some(1000) // comment this out or set to None to not bound the number of steps.
  this.keepGoing = true // comment this out if you want to stop at first exception when processing a file
}

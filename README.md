# What is MyDEL? 
	MyDEL consists of Data and Events.
		Data consists of =, {, }, String.
			String consists of all character( including digit ) except #, white space characters. 
			( but "#1" is allowed. and also " test wow" is allowed. )
		Data example
			dlc = { "power of usa" "power of rus" }
			country = {
				0 = { 
					name = USA 
					power = 100 
					province = { 0 1 2 3 4 5 }
				} 
				1 = { 
					name = RUS 
					power = 90 
					province = { 6 7 8 9 10 11 }
				}
			}
		Event ( function? ) also consist of =, {, }, String.
		Event example
			Event =
			{
			    id = 1

			    $option = { REMOVE_IF_CALL_ONESELF_EVENT } # REMOVE_IF_CALL_ONESELF_EVENT.
			    $parameter = { i j }
				$local = { x } # "NOT_DEFINED"??

				$assign_local = {
					dir = { $local.x }
					data = { 9 }
				}
				$assign_local = {
					dir = { $concat = { $ local.x } } 
					data = { 3 }
				}
			    $if = {
				condition = { $COMP<EQ = { $parameter.i $local.x } }
				then = {
				    $call = { id = { $concat = { x $add = { 1 1 } } } i = $parameter.i j = 1 }
				    $print = { value = { \n } }
				    $call = { id = 1 i = {$add = { $parameter.i 1 } } j = 1 }
				}
			    }
			}

			Event =
			{
			    id = x2
			    $parameter = { i j }
			    $option = { REMOVE_IF_CALL_ANY_EVENT }

			    $if = {
				condition = { $COMP<EQ = { $parameter.j 9 } }
				then = {
				    $print = { value = { $parameter.i } }
				    $print = { value = { " * " } }
				    $print = { value = { $parameter.j } }
				    $print = { value = { " = " } }
				    $print = { value = { $multiple = { $parameter.i $parameter.j } } }
				    $print = { value = { \n } }
				    $call = { id = x2 i = $parameter.i j = { $add = { $parameter.j 1 } } }
				}
			    }
			}

	
			
	MyDEL support if, else statements. ( $if = { condition = {  } then = {  } } $else = { then = { } } )
	
	MyDEL does not support while directly. but support using if statements and Event Recursive Call( recursive function call ).


# To Do
	1. To json or From json?
	2. Event Group?
	3. Event Graph?
	4. Fix bugs. 

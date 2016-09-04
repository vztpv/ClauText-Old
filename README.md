	
	
	# Onecard Data and Event.
	
	# Info
	Info = { CARDNUM = 52 PLAYER_NUM = 4 } # cardnum - no count two joker card!
	
	# Turn
	Turn = { dir=true start=1 end=4 n=4 now=4 } # 초기화 Event 따로??
	
	Event = {
		# No
		id=1
		# Action
		$if = { $condition = { $EQ = { /Turn/dir true } } 
			$then = { $assign = { /Turn/now value = { $add = { $modular = { /Turn/now /Turn/n } 1 } } } }
		}	
		$else = { # if else link problem? - depth?, depth max setting?
			$then = { $assign = { /Turn/now value = { $add = { $modular = { $add = { /Turn/now $add = { /Turn/n -2 } } /Turn/n} } } } }
		}		
	}
	
	# Card
	Event = {
		id = 101
		$parameter = { i } #
		# Action  cf) Card                 <-------------------- 
		$if = { $condition = { $COMP< = { $parameter.i /Info/CARDNUM  } }
			$then = {	$insert = { /Card  value = { sha = { $divide ={$parameter.i 13} } num = { $modular={$parameter.i 13} } # no ???
							isBlackJoker = no isColorJoker = no } } 
						$call = { id = 101 i = { $add = { $parameter.i 1  } } }
			}
		}
	}
	Event = {
		id = 3
		$call = { id = 101 i = 0 } # using stack? + 몇번쨰까찌 했는가?
		# insert two joker
		$insert = { /Card value = {sha = -1 num = -1  isBlackJoker = yes isColorJoker = no } }
		$insert = { /Card value = {sha = -2 num = -2  isBlackJoker = no isColorJooker = yes } }
	}
	
	# CardList - RandomShuffle!
	Event =
	{
		id = 4
		$parameter = { i }
		# Action
		
		$if = { $condition = { $COMP< = { $parameter.i $add= { 2 /Info/CARDNUM  } } } 
			$then = {	$insert = { /CardList value = { no = { $parameter.i }  } }
						$call = { id = 4 i = { $add = { $parameter.i 1 } } } }
		}            
	}
	Event = 
	{
		id = 5
		# Action
		$call = { id = 4 i = 0 }
	}
	Event = 
	{
		id = 6
		$parameter = { n }
		# Action
		$if = { $condition = { $COMP> = { $parameter.n 0 } }
			$then = {
				$swap = { /CardList value = { $rand = { 0 $add={$parameter.n -1} } } value = { $add={$parameter.n -1} } } 
				$call = { id = 6 n = { $add={ $parameter.n -1 } } } 
			}
		} 
	}
	Event =
	{
		id = 7
		# Action
		$call = { id = 6 n = { $add = { 2 /Info/CARDNUM } } }  # cf) /info/cardNum
	}
	
	# First Card Distribution
	Event = 
	{
		id = 11
		$call = { id = 12 i = 0 }
	}
	Event =
	{
		id = 12
		$parameter = { i }
		# Action
		$if = { $condition = { $COMP< = { $parameter.i /Info/PLAYER_NUM } }
			$then = {
				$insert2 = { /PlayerCardList/$parameter.i value = { $rand ={ 0 53 }  } }
				$insert2 = { /PlayerCardList/$parameter.i value = { $rand ={ 0 53 }  } }
				$call = { id = 12 i = { $add={ $parameter.i 1 } } } 
			}
		} 
	
	}
	# PutCard
	
	
	# Rule
	
	
	# AttackPoint
	
	
	# ChangeSha
	
	
	# State
	
	
	
	# Player(common)
	
	
	
	# Person # cf) inheritance?
	#PlayerCardList =
	#{
	#}
	
	Event =
	{
		id = 8
		# inital playerCardList?
		$call = { id = 9  i = 0 }
	}
	Event =
	{
		id = 9
		$parameter = { i }
		
		$if = { condition = { $COMP< = { $parameter.i /Info/PLAYER_NUM } }
			$then = {
				$insert = { /PlayerCardList value = {  }  }
				#$make = { /PlayerCarList/$parameter.i } # ??
				$call = { id = 9 i = { $add = { $parameter.i 1 } } }
			}
		}
	}
	
	# Computer
	
	
	# Print Card
	
	
	# Print Card List
	
	
	# Total?
	Event =
	{
		id = 0	
		
		#Action
		$if = { $condition = { FALSE } 
			$then = {  } 
		}
		$else = { 
			$then = {
				$call = { id = 3 }
				$call = { id = 5 }
				$call = { id = 7 }
				$call = { id = 8 }
				$call = { id = 11 }
			}
		}
		$call = { id = 1 }
	}
	
	# Main
	Main =
	{
		$call = { id = 0 }
	
		# $on = { id = ERROR }
		# $on = { id = CONSOLE }
	}
	
	# Event : Evemt1 -> Event2 -> Event3 -> Event4
	#					-> Event2 or Event5 or Event1 or Event(for Error)?
	#

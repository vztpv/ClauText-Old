	
	
	# Onecard Data and Event.
	
	# Info
	Info = { NUM_NUM = 13 SHA_NUM = 4 CARDNUM = 52 PLAYER_NUM = 4 FIRST_CARD_NUM = 5 } # cardnum - no count two joker card!
	
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
			$then = {
					$if = { $condition = { $EQ = { $parameter.i 0 } }
						$then = { $make = { /Card } }
					}
					$make = { /Card/$parameter.i }				 # ToDo - $link = { sha $divide = ~ }  => sha = ~
					$insert2 = { dir = { /Card/$parameter.i } value = { sha = { $divide ={$parameter.i 13} } num = { $modular={$parameter.i 13} } # no ???
						isBlackJoker = no isColorJoker = no } }
					$call = { id = 101 i = { $add = { $parameter.i 1  } } }
			}
		}
	}
	Event = {
		id = 3
		$call = { id = 101 i = 0 } # using stack? + 몇번쨰까찌 했는가?
		# insert two joker
		$make = { $concat = { /Card $concat = { / /Info/CARDNUM } } }
		$make = { $concat = { /Card $concat = { / $add = { /Info/CARDNUM 1 } } } } 
		$insert2 = { dir = { $concat = { /Card $concat = { / /Info/CARDNUM } } } value = {sha = -1 num = -1  isBlackJoker = yes isColorJoker = no } }
		$insert2 = { dir = { $concat = { /Card $concat = { / $add = { /Info/CARDNUM 1 } } } }  value = {sha = -2 num = -2  isBlackJoker = no isColorJooker = yes } }
	}
	# Card String Image
	##"♤","◆","♥","♧",  "☆","★"
	## 0    1     2    3     4    5
	## K->A->.....->J->Q
	## 0  1        11 12
	CardStringImage = 
	{
		num = { K A 2 3 4 5 6 7 8 9 10 J Q }
		sha = { 
			sha_image = { ♤ ◆ ♥ ♧ }
			sha_name = { spade dia heart clover }
		}
		blackJoker = ☆
		colorJoker = ★
	}
	Event = 
	{
		// $name?
		id = 100
	
		$parameter = { sha num isBlackJoker isColorJoker }
	
		#Action
		$if = { $condition = { $EQ = { yes $parameter.isBlackJoker } }
			$then = { $print = { /CardStringImage/blackJoker } }
		}
		$if = { $condition = { $EQ = { yes $parameter.isColorJoker } }
			$then = { $print = { /CardStringImage/colorJoker } }
		}
		$if = { $condition = { $COMP< = { -1 $parameter.num } }
			$then = { 
				$print = { /CardStringImage/sha/sha_image start = { $parameter.sha } last = { $parameter.sha } }
				$print = { /CardStringImage/num start = { $parameter.num } last = { $parameter.num } }
			}
		}
	}
	# CardList - RandomShuffle!
	Event =
	{
		id = 4
		$parameter = { i }
		# Action
	
		$if = { $condition = { $COMP< = { $parameter.i $add= { 2 /Info/CARDNUM  } } }
			$then = {	
				$if = { $condition = { $EQ = { $parameter.i 0 } }
					$then = { $make = { /CardList } }
				}		
				#$make = { /CardList/$parameter.i }
				$insert2 = { dir = { /CardList } value = { $parameter.i } }
				$call = { id = 4 i = { $add = { $parameter.i 1 } } }
			}
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
				$call = { id = 13 i = 0 j = $parameter.i }
				$call = { id = 12 i = { $add={ $parameter.i 1 } } }
			}
		}
	}
	Event = 
	{
		id = 13
		$parameter = { i j }
		#Action
		$if = { $condition = { $COMP< = { $parameter.i /Info/FIRST_CARD_NUM } }
			$then = {
				$insert2 = { dir = { /PlayerCardList/$parameter.j }  value = { $pop_back = { /CardList } } } 
				$call = { 
					id = 100 
					sha = { $get = { $concat = { $concat = { /Card $concat = { / /PlayerCardList/$parameter.j/ } } /sha } } }
					num = { $get = { $concat = { $concat = { /Card $concat = { / /PlayerCardList/$parameter.j/ } } /num } } }
					isBlackJoker = { $get = { $concat = { $concat = { /Card $concat = { / /PlayerCardList/$parameter.j/ } } /isBlackJoker } } }
					isColorJoker = { $get = { $concat = { $concat = { /Card $concat = { / /PlayerCardList/$parameter.j/ } } /isColorJoker } } }
				}
				$print = { "\n" }															# back ( item, not useType! )
				$call = { id = 13 i = { $add = { $parameter.i 1 } } j = $parameter.j }
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
				$if = { $condition = { $EQ = { $parameter.i 0 } }
					$then = { $make = { /PlayerCardList } }
				}
				$make = { /PlayerCardList/$parameter.i }
				$call = { id = 9 i = { $add = { $parameter.i 1 } } }
			}
		}
	}
	
	# Computer
	
	
	# Print Card
	
	
	# Print Card List
	
	
	# Keyboard Input
	
	
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
				##$remove = { /PlayerCardList/0 } # has bug. for conscat = { ~ }
				##$remove2 =  { /PlayerCardList/0 }
				#$print = { list_name = { } start = { } last == { } }
				#$print = { list_name = { } }
				#$print = { list_name = name }
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

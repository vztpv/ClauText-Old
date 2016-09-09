	
	
	# Onecard Data and Event.
	
	# Info
	Info = { NUM_NUM = 13 SHA_NUM = 4 CARDNUM = 52 TOTAL_CARD_NUM = 54 PLAYER_NUM = 4 FIRST_CARD_NUM = 5 } # cardnum - no count two joker card!
	
	# Mode?
	Mode = { ATTACK_MODE = 1 GENERAL_MODE = 0 }
	
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
	Event = {
		#No
		id = 200
		#Action
		$if = { $condition = { $EQ = { /Turn/dir true } } 
			$then = { $assign = { /Turn/dir false } }
		}
		$else = {
			$then = { $assign = { /Turn/dir true } }
		}
	}
		# Turn initial Event
	Event = {
		id = 201
	
		$assign = { /Turn/dir value = { true } }
		$assign = { /Turn/start value = { 1 } }
		$assign = { /Turn/end value = { $get = { /Info/PLAYER_NUM } } }
		$assign = { /Turn/n value = { $get = { /Info/PLAYER_NUM } } }
		$assign = { /Turn/now value = { $get = { /Info/PLAYER_NUM } } }
	}
	
	# Card
	Event = { # just one!
		id = 101
		$parameter = { i } #
		# Action  cf) Card                 <--------------------
		$if = { $condition = { $COMP< = { $parameter.i /Info/CARDNUM  } }
			$then = {
					$if = { $condition = { $EQ = { $parameter.i 0 } }
						$then = { $make = { /Card } }
					}
					$make = { /Card/$parameter.i }
					$insert2 = { dir = { /Card/$parameter.i } value = { sha = { $divide ={$parameter.i 13} } num = { $modular={$parameter.i 13} } # no ???
						isBlackJoker = no isColorJoker = no } }
					$call = { id = 101 i = { $add = { $parameter.i 1  } } }
			}
		}
	}
	Event = { # just one!
		id = 3
		$call = { id = 101 i = 0 } # using stack? + 몇번쨰까찌 했는가?
		# insert two joker
		$make = { $concat = { /Card $concat = { / /Info/CARDNUM } } }
		$make = { $concat = { /Card $concat = { / $add = { /Info/CARDNUM 1 } } } } 
		$insert2 = { dir = { $concat = { /Card $concat = { / /Info/CARDNUM } } } value = {sha = -1 num = -1  isBlackJoker = yes isColorJoker = no } }
		$insert2 = { dir = { $concat = { /Card $concat = { / $add = { /Info/CARDNUM 1 } } } }  value = {sha = -2 num = -2  isBlackJoker = no isColorJoker = yes } }
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
		blackJoker = { ☆ }
		colorJoker = { ★ }
	}
	Event = 
	{
		// $name?
		id = 100
	
		$parameter = { sha num isBlackJoker isColorJoker }
	
		#Action
		$if = { $condition = { $EQ = { yes $parameter.isBlackJoker } }
			$then = { $print = { value = { /CardStringImage/blackJoker } } } 
		}
		$if = { $condition = { $EQ = { yes $parameter.isColorJoker } }
			$then = { $print = { value = { /CardStringImage/colorJoker } } } 
		}
		$if = { $condition = { $COMP< = { -1 $parameter.num } }
			$then = { 
				$print = { dir = { /CardStringImage/sha/sha_image } start = { $parameter.sha } last = { $parameter.sha } }
				$print = { dir = { /CardStringImage/num } start = { $parameter.num } last = { $parameter.num } }
			}
		}
	}
	# CardList 
	Event = # just one!
	{
		id = 4
		$parameter = { i }
		# Action
	
		$if = { $condition = { $COMP< = { $parameter.i $add= { 2 /Info/CARDNUM  } } }
			$then = {	
				$if = { $condition = { $EQ = { $parameter.i 0 } }
					$then = { $make = { /CardList } }
				}		
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
																			# back ( item, not useType! )
				$call = { id = 13 i = { $add = { $parameter.i 1 } } j = $parameter.j }
			}
		}	
	}
	
	# Card AllReCall to CardList?
		# card to cardlist?
	
	# PutCard
	PutCard = { cardId = NONE }
		# assign
	Event = {
		id = 20
		$parameter = { card_id }
		$assign = { /PutCard/cardId value = { $parameter.card_id } }
	}
		# clear
	Event = {
		id = 21
		$assign = { /PutCard/cardId  value = { NONE } }
	}
	
	# PutCard, move to card list!
	Event = {
		id = 22
		$insert2 = { dir = { /CardList } value = { $get = /PutCard/cardId } }
		$call = { id = 21 }
	}
	
	# Rule
		#Onecard Fucntions No.
	FunctionNo = { }
		#build functionNo
	Event = {
		id = 1000
	
		$parameter = { i }
	
		$if = { $condition = { $COMP< = { $parameter.i /Info/TOTAL_CARD_NUM } } 
			$then = { 
			#	$print = { value = { "start" } }
			#	$print = { value = { \n } }
				$if = { $condition = { $EQ = { $get = { $concat = { $concat = { /Card $concat = { /  $parameter.i } } /isBlackJoker } } yes } }
					$then = { $insert2 = { dir = { /FunctionNo } value = { 3 } }
			#								$print = { value = { "0" } }
											# $call = { id = 1000 i = { $add = { $parameter.i 1 } } }
											#	$return = { }
												 }
				}
				$else = {
					$then = {
						$if = { $condition = { $EQ = { $get = { $concat = { $concat = { /Card $concat = { /  $parameter.i } } /isColorJoker } } yes } }
							$then = { $insert2 = { dir = { /FunctionNo } value = { 4 } }
			#								$print = { value = { "1" } }
												#$call = { id = 1000 i = { $add = { $parameter.i 1 } } }
												#$return = { } 
												}
						}
						$else = {
							$then = { 
								$if = { $condition =  { $AND = { $EQ = { $get = { $concat = { $concat = { /Card $concat = { /  $parameter.i } } /sha } } 0 } 
																 $EQ = { $get = { $concat = { $concat = { /Card $concat = { /  $parameter.i } } /num } } 1 } } }
									$then = { $insert2 = { dir = { /FunctionNo } value = { 9 } }
			#								$print = { value = { "2" } }
												# $call = { id = 1000 i = { $add = { $parameter.i 1 } } }
												#$return = { }
												}
								}
								$else = {
									$then = {
										$if = { $condition = { $EQ = { $get = { $concat = { $concat = { /Card $concat = { /  $parameter.i } } /num } } 0 } }
											$then = { 
			#								$print = { value = { "3" } }
												$insert2 = { dir = { /FunctionNo } value = { 8 } }
						
												$call = { id = 1000 i = { $add = { $parameter.i 1 } } }	
												$return = { }
											}
										}
										$if = { $condition = { $EQ = { $get = { $concat = { $concat = { /Card $concat = { /  $parameter.i } } /num } } 1 } }
											$then = {
			#								$print = { value = { "3" } }
												$insert2 = { dir = { /FunctionNo } value = { 1 } }
								
												$call = { id = 1000 i = { $add = { $parameter.i 1 } } }		
												$return = { }
											}
										}
										$if = { $condition = { $EQ = { $get = { $concat = { $concat = { /Card $concat = { /  $parameter.i } } /num } } 2 } }
											$then = { 
			#								$print = { value = { "3" } }
												$insert2 = { dir = { /FunctionNo } value = { 2 } }
								
												$call = { id = 1000 i = { $add = { $parameter.i 1 } } }
												$return = { }
											}
										}
										$if = { $condition = { $EQ = { $get = { $concat = { $concat = { /Card $concat = { /  $parameter.i } } /num } } 7 } }
											$then = { 
			#								$print = { value = { "3" } }
												$insert2 = { dir = { /FunctionNo } value = { 5 } }
								
												$call = { id = 1000 i = { $add = { $parameter.i 1 } } }							
												$return = { }
											}
										}
										$if = { $condition = { $EQ = { $get = { $concat = { $concat = { /Card $concat = { /  $parameter.i } } /num } } 11 } }
											$then = { 
			#								$print = { value = { "3" } }
												$insert2 = { dir = { /FunctionNo } value = { 6 } }
								
												$call = { id = 1000 i = { $add = { $parameter.i 1 } } }			
												$return = { }
											}
										}
										$if = { $condition = { $EQ = { $get = { $concat = { $concat = { /Card $concat = { /  $parameter.i } } /num } } 12 } }
											$then = { 
			#								$print = { value = { "3" } }
												$insert2 = { dir = { /FunctionNo } value = { 7 } }
								
												$call = { id = 1000 i = { $add = { $parameter.i 1 } } }
												$return = { }
											}
										}
			#							$if = { $condition = { FALSE } 
			#								$then = { $print = { value = { "|" } } }
			#							}
			#							$if = { $condition = { TRUE }
			#								$then = { $print = { value = { "/" } } }
										
			#							}
										$insert2 = { dir = { /FunctionNo } value = { 0 } }
										
			#								$print = { value = { "4" } }
									}
								}
							}
						}
					}
				}
			#	$print = { value = { \n } }
				$call = { id = 1000 i = { $add = { $parameter.i 1 } } }
			}
		}
	}
	Event = {
		id = 1001
	
		$call = { id  = 1000 i = 0 }
	}
		#Onecard main rule Event
	Event= {
		id = 1005
	
		$parameter = { Old_num New_num Old_sha New_sha Old New Mode }
	
		$if = { $condition = { $EQ = { $parameter.Mode /Mode/ATTACK_MODE } }
			$then = { 
				$if = { $condition = { $OR = { $EQ = { $parameter.Old 3 } EQ = { $parameter.Old 4 } } } 
					$then ={ 
						$if = { $condition = { $EQ = { $parameter.New 3 } } 
							$then = {
								$return = { TRUE }
							}	
						}
					}
				}
			}
		}
		$return = { FALSE }
	}
	
	# AttackPoint
	AttackPoint = { total_point = 0 now_point = 0 }
		# point incerase
		# point reset
	
	# ChangeSha
	ChangeSha = { sha = NONE }
	
	# State
	State = { mode = /mode/GENERAL_MODE }
	
	
	# Person 
		# io?
	Person = 
	{
	
	}
	
	
	# Computer
		# AI?
	Computer =
	{
	
	}
	
	
	# Player(common)
		# name list
		# card list
		# game over : yes or no list
	PlayerNameList =
	{
	
	}
	PlayerGameOverList = 
	{
	
	}
	
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
	
	
	
	# Print Card ??
	
	
	# Print Card List
	Event = 
	{
		id = 50
		$parameter = { i }
	
		$if = { $condition = { $COMP< = { $parameter.i /Info/PLAYER_NUM } }
			$then = {
				$call = { 
					id = 51
					i = $parameter.i
					j = 0
				}
				$print = { value = { \n } }		
				$call = {
					id = 50
					i = { $add= { $parameter.i 1 } }
				}
			}
		}
	}
	Event = 
	{
		id = 51
		$parameter = { i j }
	
		$if = { $condition = { $COMP< = { $parameter.j $size = { /PlayerCardList/$parameter.i } } }
			$then = {
				$call = { 
					id = 100
					sha = { $get= {$concat = { $concat = { /Card $concat = { / $element = { /PlayerCardList/$parameter.i $parameter.j } } } /sha } } }
					num = { $get= {$concat = { $concat = { /Card $concat = { / $element = { /PlayerCardList/$parameter.i $parameter.j } } } /num } } }
					isBlackJoker = { $get= {$concat = { $concat = { /Card $concat = { / $element = { /PlayerCardList/$parameter.i $parameter.j } } } /isBlackJoker } } } 
					isColorJoker = { $get= {$concat = { $concat = { /Card $concat = { / $element = { /PlayerCardList/$parameter.i $parameter.j } } } /isColorJoker } } }
				}
				$print = { value = { "/" } }		
				$call = {
					id = 51
					i = $parameter.i
					j = { $add= { $parameter.j 1 } }
				}
			}
		}
	}
	
	# Keyboard Input
		#char?
		#int?
		#string?
	
	# Pause
	
	
	# Total?
	Event =
	{
		id = 0
		$local = { result }
		#Action
		$if = { $condition = { FALSE }
			$then = {  }
		}
		$else = {
			$then = {
				$call = { id = 201 } # init Turn.
				$call = { id = 3 }
				$call = { id = 5 }
				$call = { id = 7 }
				$call = { id = 8 }
				$call = { id = 11 }
	
				$call = { id = 50 i = 0 } # debug test print!
				// test
				$call = { id = 1001 }
				$call = { id = 1005  Old_num  = 0 New_num = 0 Old_sha = 0 New_sha = 0 Old = 0 New = 0 Mode = /Mode/GENERAL_MODE  }
				$assign = { $local.result value = { $return_value = { } } }
				$print = { value = { $local.result } }
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

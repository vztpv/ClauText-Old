	
	
	## id -> name?
	# Onecard Data and Event.
	
	# Info
	Info = { NUM_NUM = 13 SHA_NUM = 4 CARDNUM = 52 TOTAL_CARD_NUM = 54 PLAYER_NUM = 4 FIRST_CARD_NUM = 5
		GAME_OVER_CARD_NUM = 15  ONE_CARD_FAIL_CARD_NUM = 5
	 } # cardnum - no count two joker card!
	
	
	# Mode?
	Mode = { JUMP_MODE = 6 BACK_MODE = 7 MORE_ONE_MODE = 8 CHANGE_SHA_MODE = 3 
			 GAME_END_MODE = 4 GAME_OVER_MODE = 5 ATTACK_MODE = 1 GENERAL_MODE = 0 }
	
	# Turn
	Turn = { dir=true start=1 end=4 n=4 now=4 } # 초기화 Event 따로??
	
	Event = {
		# No
		id=1
		# Action
		$if = { $condition = { $EQ = { /Turn/dir true } }
			$then = { 
				$assign = { /Turn/now value = { $add = { $modular = { /Turn/now /Turn/n } 1 } } }
			}
		}
		$else = { # if else link problem? - depth?, depth max setting?
			$then = { 
				$assign = { /Turn/now value = { $add = { $modular = { $add = { /Turn/now $add = { /Turn/n -2 } } /Turn/n } 1 } } } 
			}
		}
	}
	Event = {
		#No
		id = 200
		#Action
		$if = { $condition = { $EQ = { /Turn/dir true } } 
			$then = { $assign = { /Turn/dir value = { false } } }
		}
		$else = {
			$then = { $assign = { /Turn/dir value = { true } } }
		}
	}
		# Turn initial Event
	Event = {
		id = 201
	
		$assign = { /Turn/dir value = { true } }
		$assign = { /Turn/start value = { 1 } }
		$assign = { /Turn/end value = { /Info/PLAYER_NUM } } 
		$assign = { /Turn/n value = { /Info/PLAYER_NUM } } 
		$assign = { /Turn/now value = { /Info/PLAYER_NUM } } 
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
	Event = {
		id = 103
		$parameter = { card }
	
		$return = { $divide = { $parameter.card 13 } }
	}
	Event = {
		id = 104
		$parameter = { card }
	
		$return = { $modular = { $parameter.card 13 } }
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
				$call = { id = 6 n = { $add = { $parameter.n -1 } } }
			}
		}
	}
	Event =
	{
		id = 7
		# Action
		$call = { id = 6 n = { $size = { /CardList } } }  # cf) /info/cardNum
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
		$insert2 = { dir = { /CardList } value = { /PutCard/cardId } }
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
					$then = { 
						$insert2 = { dir = { /FunctionNo } value = { 3 } }
			#								$print = { value = { "0" } }
											# $call = { id = 1000 i = { $add = { $parameter.i 1 } } }
											#	$return = { }
												 }
				}
				$else = {
					$then = {
						$if = { $condition = { $EQ = { $get = { $concat = { $concat = { /Card $concat = { /  $parameter.i } } /isColorJoker } } yes } }
							$then = { 
								$insert2 = { dir = { /FunctionNo } value = { 4 } }
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
		#Onecard main rule Event : CanPut?
	Event = {
		id = 1005
	
		$parameter = { Old_num New_num Old_sha New_sha Old New Mode }
	
	
		$if = { $condition = { $EQ = { $parameter.Mode /Mode/ATTACK_MODE } }
			$then = { 
				$if = { $condition = { $OR = { $EQ = { $parameter.Old 3 } $EQ = { $parameter.Old 4 } } } 
					$then ={ 
						$if = { $condition = { $EQ = { $parameter.New 3 } } 
							$then = {
								$return = { TRUE }
							}	
						}
						$if = { $condition = { $EQ = { $parameter.New 4 } } 
							$then = {
								$return = { TRUE }
							}
						}
						$if = { $condition = { $EQ = { $parameter.New 9 } } 
							$then = {
								$return = { TRUE }
							}
						}
						$return = { FALSE }
					}
				}
				$if = { $condition = { $OR = { $EQ = { $parameter.Old 1 } $EQ = { $parameter.Old 9 } } } 
					$then = {
						$if = { $condition = { $OR = { $EQ = { $parameter.New 1 } $EQ = { $parameter.New 9 } } } 
							$then = {
								$return = { TRUE }
							}
						}
						$if = { $condition = { $AND = { $EQ = { $parameter.Old_sha $parameter.New_sha } 
													   $EQ = { $parameter.New 2 } } } 
							$then = {
								$return = { TRUE }
							}
						}
						$if = { $condition = { $EQ = { $parameter.New 3 } } 
							$then = {
								$return = { TRUE }
							}
						}
						$if = { $condition = { $EQ = { $parameter.New 4 } } 
							$then = {
								$return = { TRUE }
							}
						}
						$return = { FALSE }
					}
				}
				$if = { $condition = { $EQ = { $parameter.Old 2 } }
					$then = {
						$if = { $condition = { $EQ = { $parameter.New 2 } } 
							$then = {
								$return = { TRUE }
							}
						}
						$if = { $condition = { $AND = { $EQ = { $parameter.Old_sha $parameter.New_sha } 
														$EQ = { $parameter.New 1 } } }
							$then = {
								$return = { TRUE }
							}
						}
						$if = { $condition = { $AND = { $EQ = { $parameter.Old_sha $parameter.New_sha } 
														$EQ = { $parameter.New 9 } } }
							$then = {
								$return = { TRUE }
							}
						}
						$if = { $condition = { $EQ = { $parameter.New 3 } } 
							$then = {
								$return = { TRUE }
							}
						}
						$if = { $condition = { $EQ = { $parameter.New 4 } } 
							$then = {
								$return = { TRUE }
							}
						}
						$return = { FALSE }
					}
				}
				#$return = { FALSE }
			}
		}
		$else = {
			$then = {
				$if = { $condition = { $EQ = { $parameter.New 3 } } 
					$then = {
						$return = { TRUE }
					}
				}
				$if = { $condition = { $EQ = { $parameter.New 4 } } 
					$then = {
						$return = { TRUE }
					}
				}
				$if = { $condition = { $OR = 
									{ $NOT = { $AND = { 
									$NOTEQ = { $parameter.Old_sha $parameter.New_sha } 
									$NOTEQ = { $parameter.Old_num $parameter.New_num } } 
									}  $AND = { $NOTEQ = { $parameter.Mode /Mode/ATTACK_MODE } 
												$OR = { $EQ = { $parameter.Old 3 }
												        $EQ = { $parameter.Old 4 } }
									}
						   } 		
					}
					$then = {
						$return = { TRUE }
					}
				}
			}
		}
		$return = { FALSE }
	}
	Event = {
		id = 1006
	
		$parameter = { put_card card mode }
		$local = { Old_num New_num Old_sha New_sha Old New }
	
		$call = { id = 103 card = $parameter.put_card }
		$assign = { $local.Old_sha value = { $return_value = { } } }
	
		$if = { 
			$condition = { $EQ = { $parameter.mode /Mode/CHANGE_SHA_MODE } }
			$then = {
				$assign = { $local.Old_sha value = { /ChangeSha/sha } }
			}
		}
		$call = { id = 104 card = $parameter.put_card }
		$assign = { $local.Old_num value = { $return_value = { } } }
		
		$call = { id = 103 card = $parameter.card }
		$assign = { $local.New_sha value = { $return_value = { } } }
		$call = { id = 104 card = $parameter.card }
		$assign = { $local.New_num value = { $return_value = { } } }
	
		$assign = { $local.Old value = { $element = { /FunctionNo $parameter.put_card } } }
		$assign = { $local.New value = { $element = { /FunctionNo $parameter.card } } }
	
	
		$call = { id = 1005 Old_num = { $local.Old_num } New_num = { $local.New_num }  
							Old_sha = { $local.Old_sha } New_sha = { $local.New_sha }  
					Old = { $local.Old }  New = { $local.New }  Mode = { $parameter.mode }
		}
	
		$return = { $return_value = { } }
	}
	
	# Functions
	Functions = {
	
	}
	Event = {
		id = 1007
	
		$local = { Attack_A1 KK1 Attack_A2 Attack_A3 Attack_A4 Attack_A5 Change_Sha1 Jump1 Back1 }
	
		#NONE
		$make = { /Functions/0 }
		$insert2 = { dir = { /Functions/0 } value = { attack_point= 0 kk = no change_sha = no jump = no back = no } }
	
		$assign = { $local.Attack_A2 value = { attack_point = 1 kk = no change_sha = no jump = no back = no } }
		$make = { /Functions/1 }
		$insert2 = { dir = { /Functions/1 } value = { $local.Attack_A2 } }
		
		$assign = { $local.Attack_A3 value = { attack_point = 2 kk = no change_sha = no jump = no back = no } }
		$make = { /Functions/2 }
		$insert2 = { dir = { /Functions/2 } value = { $local.Attack_A3 } }
		
		$assign = { $local.Attack_A4 value = { attack_point = 5 kk = no change_sha = no jump = no back = no } }
		$make = { /Functions/3 }
		$insert2 = { dir = { /Functions/3 } value = { $local.Attack_A4 } }
		
		$assign = { $local.Attack_A5 value = { attack_point = 7 kk = no change_sha = no jump = no back = no } }
		$make = { /Functions/4 }
		$insert2 = { dir = { /Functions/4 } value = { $local.Attack_A5 } }
		
		$assign = { $local.Change_Sha1 value = { attack_point = 0 kk = no change_sha = yes jump = no back = no } } 
		$make = { /Functions/5 }
		$insert2 = { dir = { /Functions/5 } value = { $local.Change_Sha1 } }
		
		$assign = { $local.Jump1 value = { attack_point = 0 kk = no change_sha = no jump = yes back = no } }
		$make = { /Functions/6 }
		$insert2 = { dir = { /Functions/6 } value = { $local.Jump1 } }
		
		$assign = { $local.Back1 value = { attack_point = 0 kk = no change_sha = no jump = no back = yes } } 
		$make = { /Functions/7 }
		$insert2 = { dir = { /Functions/7 } value = { $local.Back1 } }
	
		$assign = { $local.KK1 value = { attack_point = 0 kk = yes change_sha = no jump = no back = no } }
		$make = { /Functions/8 }
		$insert2 = { dir = { /Functions/8 } value = { $local.KK1 } }
	
		$assign = { $local.Attack_A1 value = { attack_point = 3 kk = no change_sha = no jump = no back = no }  }
		$make = { /Functions/9 }
		$insert2 = { dir = { /Functions/9 } value = { $local.Attack_A1 } }
	}
	
	Event = {
		id = 1008
	
		$parameter = { card }
		$local = { card_function_no function_dir }
	
		#debug
		$print = { value = { $parameter.card } }
	
		# card -> card_function_no ( access table )
		$assign = { $local.card_function_no value = { $element = { /FunctionNo $parameter.card } } }
	
		# function_dir <= Functions/i/ # can?? $get?
		$assign = { $local.function_dir value = { $concat = { /Functions $concat = { / $local.card_function_no } } } }
	
		$print = { vallue = { $local.function_dir } }
		# / + $local.function_dir + attack_point > 0
		$if = { $condition = { $COMP> = { $get = { $concat = { $local.function_dir $concat = { / attack_point } } } 0 } } 
			$then = {
				$call = { id = 1010 point = { $get = { $concat = { $local.function_dir $concat = { / attack_point } } } } }
			}
		}
		#			   + change_sha = yes ? 
		$if = { $condition = { $EQ = { $get = { $concat = { $local.function_dir $concat = { / change_sha } } } yes } } 
			$then = {
				$call = { id = 1015 }
			}
		}
		#			   + jump = yes?
	 	$if = { $condition = { $EQ = { $get = { $concat = { $local.function_dir $concat = { / jump } } } yes } } 
			$then = {
				$call = { id = 1016 i = 0 }
			}
		}
		#			   + back = yes?
		$if = { $condition = { $EQ = { $get = { $concat = { $local.function_dir $concat = { / back } } } yes } } 
			$then = {
				$call = { id = 1017 }
			}
		}
		## todo - kk
		$if = { 
			$condition = { $EQ = { $get = { $concat = { $local.function_dir $concat = { / kk } } } yes } }  
			$then = {
				$call = { id = 1017 }
				$call = { id = 1016 i = 0 }
				$call = { id = 1017 } 
			}
		}
	}
		# change_sha
	Event = {
		id = 1015
	
		# person? - $input = { } # todo!
		# computer? - random!
		$if = { $condition = { $EQ = { /Turn/now /State/person_stream } } # start at 1
			$then = {
				# todo - person play!
			}	
		}
		$else =	{
			$then = {
				$assign = { /ChangeSha/sha value = { $rand = { 0 3 } } }
				$assign = { /State/mode value = { /Mode/CHANGE_SHA_MODE } }
				
				#debug..
				$print = { value = { "change sha : " } }
				$print = { value = { /ChangeSha/sha } }
				$print = { value = { \n } }
			}
		}
	}
		# jump, next with gameover list.
	Event = {
		id = 1016
	
		$parameter = { i }
		# with PlayerGameOverList
	
		$call = { id = 1 }
	
		$if = { 
			$condition = { $EQ = { $element = { /PlayerGameOverList $add = { /Turn/now -1 } } no } }
			$then = {
				$return = { }
			}
		}
		$else = {
			$then = {
				$call = { id = 1016 i = { $add = { $parameter.i 1 } } }
			}
		}
	}
		# back
	Event = {
		id = 1017
	
		$call = { id = 200 }
	}
	
	# AttackPoint
	AttackPoint = { total_point = 0 now_point = 0 }
		# point incerase - id 1010?
	Event = {
		id = 1010
	
		$parameter = { point }
	
		$assign = { /AttackPoint/now_point value = { $parameter.point } }
		$assign = { /AttackPoint/total_point value = { $add = { /AttackPoint/total_point $parameter.point } } }
		$assign = { /State/mode value = { /Mode/ATTACK_MODE } }
	}
		# point reset - id 1011?
	Event = {
		id = 1011
	
		$assign = { /AttackPoint/now_point value = { 0 } }
		$assign = { /AttackPoint/total_point value = { 0 } }
	}
	
	# ChangeSha
	ChangeSha = { sha = NONE }
	
	# State
	State = {
	
	}
		# init? using insert2?
	Event =
	{
		id = 1100
	
		$insert2 = { dir = { /State } value = { mode = { /Mode/GENERAL_MODE } } } ## why? cf) mode = /Mode/GENEREAL_MODE
		# add action_state? = NONE? EAT? PUT?
		$insert2 = { dir = { /State } value = { action_state = NONE } }
		$insert2 = { dir = { /State } value = { action_card = NONE } }
		# add person_stream? turn?
		$insert2 = { dir = { /State } value = { person_stream = -1 } }
	}
	
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
	Event =
	{
		id = 1200
	
		$parameter = { }
	}
	
	# Player(common)
		# name list
		# card list - done.
		# game over : yes or no list
		# next Turn
		# back
	PlayerNameList =
	{
	
	}
	PlayerGameOverList = 
	{
	
	}
	Event = {
		id = 3003
		
		$call = { id = 3002 i = 0 }
	}
	Event = {
		id = 3002
	
		$parameter = { i } 
	
		$if = { $conditioin = { $COMP< = { $parameter.i /Info/PLAYER_NUM } }
			$then = { 
				$insert2 = { dir = { /PlayerGameOverList } value = { no } }
				$call = { id = 3002 i = { $add = { $parameter.i 1 } }  }
			}
		}
	}
	Event = {
		id = 3001
	
		$call = { id = 3000 i = 0 j = 1 }
	}
	Event = {
		id = 3000
		
		$parameter = { i j }
		
		$if = {
			$condition = { $COMP< = { $parameter.i /Info/PLAYER_NUM } } 	
			$then = {
				$if = { $condition = { $NOTEQ = { $parameter.i /State/person_stream } }  
					$then = { 
						$insert2 = { dir = { /PlayerNameList } value = { $concat = { computer  j } } } 
						$call = { id = 3000 i = { $add = { $parameter.i 1 } } j = { $add = { $parameter.j 1 } } }
					}
				}
				$else = {
					$then = {
						$insert2 = { dir = { /PlayerNameList } value ={ person } } 
						$call = { id = 3000 i = { $add = { $parameter.i 1 } } j = $parameter.j }
					}
				}
			}
		}
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
	
	Event =
	{
		id = 52 
	
		$parameter = { card }
	
		$call = { 
			id = 100
			sha = { $get= {$concat = { $concat = { /Card $concat = { / $parameter.card } } /sha } } }
			num = { $get= {$concat = { $concat = { /Card $concat = { / $parameter.card } } /num } } }
			isBlackJoker = { $get= {$concat = { $concat = { /Card $concat = { / $parameter.card } } /isBlackJoker } } } 
			isColorJoker = { $get= {$concat = { $concat = { /Card $concat = { / $parameter.card } } /isColorJoker } } }
		}
	}
	
	
	# Print Card ??
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
	
	# Keyboard Input
		#char?
		#int?
		#string?
		# $input = { } # cin >> string_type;
		# $_getch = { } # _getch();
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
				$call = { id = 21 }
				#
				$assign = { /PutCard/cardId value = { $pop_back = { /CardList } } }
	
				$call = { id = 50 i = 0 } # debug test print!
	
				// test
				$call = { id = 1001 }
				$call = { id = 1006  put_card = 0 card = 0 mode = { /Mode/GENERAL_MODE  } }
				$assign = { $local.result value = { $return_value = { } } }
				$print = { value = { $local.result } }
				//~test
	
				$call = { id = 1100 }
				$call = { id = 1011 }
				
				$call = { id = 3001 }
				$call = { id = 3003 }
				$call = { id = 1007 }
				// main call?
				$call = { id = 4001 }
			}
		}
	}
	
	Event = {
		id = 4001
		
		$option = { REMOVE_NOW_EVENT_STACK_A }
	
		$call = { id = 4000 }
	
		$if = { $Condition = { $NOTEQ = { $return_value = { }  /Mode/GAME_END_MODE } }
			$then = { 
				$call = { id = 4001 }
			}
		}
	}
	
	Event = {
		id = 4000
	
		$local = { card idx }
	
			#$clear_screen = { } # system( "cls" ); # in windows.
		$clear_screen = { }
			# print turn
		$print = { value = { /Turn/now } }
		$print = { value = { \n } }
		$print = { value = { /State/mode } } 
		$print = { value = { \n } }
			# print player`s cards - $parameter = { id i n }  # n==i
		$call = { id = 50 i = 0 }
				# print my card list
			# print putCard
		$call = { id = 52 card = /PutCard/cardId }
		$print = { value = { \n } }
			# print ?.
				#$print = { value = { "? : " } }
				# input = 먹는다. 0, 그외 1~n
				# input = $getch = { } # ?
			# Card Put or Eat # <- state/action_state == PUT or EAT?
		$if = { # computer
			$condition = { $NOTEQ = { /Turn/now /State/person_stream } }
			$then = {
				$assign = { /State/action_state  value = { EAT } }
				$call = { id = 4010 i = 0 n = 20 } # replace??
				#$print = { value = { $local.card } }
	
				$if = { 
					$conditio = { $EQ = { /State/action_state PUT } }
					$then = { 
						$assign = { $local.card value = { $return_value = { } } }
						$call = { id = 52 card = $local.card }
						$print = { value = { \n } }
					}
				}
			}	
		}
		$else = { # person
			$then = {
				#
			}
		}
	
		$print = { value = { /State/action_state } }
		$if = {
			$condition = { $EQ = { /State/action_state PUT } } # action_state initial event??
			$then = {
				$assign = { /State/action_state value = { -1 } }
				
				$call = { id = 1006 put_card = /PutCard/cardId card = $local.card mode = /State/mode }
				$if = {
					$condition = { $EQ = { FALSE $return_value = { } } }
					$then =  {
						// for person!
						$return = { /State/mode }
					}
				}
				# one card chk
				$if = {
					$condition = { $EQ = { 2 $size = { $concat = { /PlayerCardList $concat = { 
												/ $add = { -1 /Turn/now } } } } } }
					$then = {
						# one card event
						$call = { id = 4011 card = $local.card }
						
						# next turn
						$call = { id = 1016 i = 0 }
						$return = { /State/mode }
					}
				}	
				$insert2 = { dir = { /CardList } value = { /PutCard/cardId } }
				# mix
				$call = { id = 7 }
				
				#  findindex( card )
				$findIndex = { 
					dir = { $concat = { /PlayerCardList $concat = { / $add = { /Turn/now -1 } } } }
					value = { $local.card } 
				}
				$assign = { $local.idx value = { $return_value = { } } }
					#putcard = getcard(idx);
				$assign = { /PutCard/cardId value = { $element = { $concat = { /PlayerCardList $concat = { / $add = { /Turn/now -1 } } } $local.idx } } }
				$remove3 = {
					dir = { $concat = { /PlayerCardList $concat = { / $add = { /Turn/now -1 } } } }
					value = { $local.idx }
				}
	
				# gameOver chk
				$if = {
					$condition = { $EQ = { 0 $size = { $concat = { /PlayerCardList $concat = { / $add = { -1 /Turn/now } } } } } }
					$then = {
						$assign = { /State/mode value = { /Mode/GAME_END_MODE } }
						$return = { /State/mode }
					}
				}
	
				# end of change sha
				$if = {
					$condition = { $EQ = { /State/mode /Mode/CHANGE_SHA_MODE } }
					$then = {
						$assign = { /State/mode value = { /Mode/GENERAL_MODE } }
					}
				}
				
				# card effect.
				$call = { id = 1008 card = $local.card }
			}
		}
		$if = {
			$condition = { $EQ = { /State/action_state EAT } }
			$then = {
				$assign = { /State/action_state value = { -1 } }
				# 4013 - Eat
				$if = { 
					$condition = { $EQ = { /State/mode /Mode/ATTACK_MODE } } 
					$then = {
						$call = { id = 4013 i = 0 n = /AttackPoint/total_point }
						$assign = { /State/mode value ={ /Mode/GENERAL_MODE } }
						$call = { id = 1011 }
					}
				}
				$else = {
					$then = {
						$call = { id = 4013 i = 0 n = 1 }
					}
				}
				# 게임오버 체크 + 전체게임오버 체크
				$if = {
					$condition = { $COMP< = { $add = { /Info/GAME_OVER_CARD_NUM -1 } $size = { $concat = { /PlayerCardList $concat = { / $add = { /Turn/now -1 } } } } } }
					$then = {
						# 1명만 살아 남은 경우
						$assign = { /Temp/count value = { 0 } }
						$call = { id = 4014 i = 0 n =  /Info/PLAYER_NUM }
						$if = {
							$condition = { $EQ = { /Temp/count 2 } }
							$then = { 
								$assign = { /State/mode value = { /Mode/GAME_END_MODE } }
								$return = { /State/mode }
							}
						}
						$assign = { /Temp/count value = { 0 } }
						
						# set gameOver to yes! #주의  TRUE FALSE / yes no / true false
						$setElement = { 
							dir = { /PlayerGameOverList } 
							idx = { $add = { -1 /Turn/now } } 
							value = { yes } 
						}
						
						# 카드를 카드리스트에 넣는다.
						$call = { id = 4015 }
						
						# mix
						$call = { id = 7 }
					}
				}
			}
		}
			# print some string?
			# $getch() = { } # ?
		$_getch = { }
			# Next Turn...
		$call = { id = 1016 i = 0 }
		$return = { /State/mode }
	}
	
	Temp = # ???
	{
		count = 0
	}
	
	Event =
	{
		id = 4015
	
		$local = { index card }
		
		$if = {
			$condition = { $COMP< = { 0 $size = { $concat = { /PlayerCardList
														 $concat = { / $add = { -1 /Turn/now } } } } } }
			$then = {
				$assign = { $local.index value = { $add = { -1 $size = { $concat = { 
							/PlayerCardList $concat = { / $add = { -1 /Turn/now } } } } } } }
				$assign = { $local.card value = { $element = { $concat = { /PlayerCardList $concat = { / $add = { -1 /Turn/now } } } $local.index } } }
				$insert2 = { dir = { /CardList } value = { $local.card } }
				$remove3 = { 
					dir = { $concat = { /PlayerCardList $concat = { / $add = { -1 /Turn/now } } } }
				 	idx = { $local.index }
				}
				$call = { id = 4015 }
			}
		}
	}
	
	Event = 
	{
		id = 4014
		
		$parameter = { i n }
		# return count.
	
		$if = { 
			$condition = { $COMP< = { $parameter.i $parameter.n } }
			$then = {
				$if = { 
					$condition = { $EQ = { yes $element = { /PlayerGameOverList $parameter.i } } }
					$then = {
						$assign = { /Temp/count value = { $add = { /Temp/count 1 } } }
					}
				}
				$call = { id = 4014 i = { $add = { $parameter.i 1 } } n = parameter.n }
			}
		}
	}
	
	Event =
	{
		# name for ~ { Eat }
		id = 4013
	
		$parameter = { i n }
	
		$if = {
			$condition = { $COMP< = { $parameter.i $parameter.n } }
			$then = {
				$call = { id = 4012 }
				$call = { id = 4013 i = { $add = { $parameter.i 1 } } n = $parameter.n }
			}
		}
	}
	
	Event =
	{
		# name = Eat
		id = 4012
	
		$local = { card }
	
		$if = {
			$condition = { $EQ = { 0 $size = { /CardList } } }
			$then = { 
				$return = { "ERROR_cardList_is_empty" } # chk.. $isError = { $return_value = { } }
			}
		}
		$assign = { $local.card value = { $pop_back = { /CardList } } }
		$insert2 = {
			 dir = { $concat = { /PlayerCardList $concat = { / $add ={ /Turn/now -1 } } } } 
	  		 value = { $local.card  } 
		}
	}
	Event =
	{
		id = 4011
		
		$parameter = { card }
	
		$local = { select idx }
		
		$if = { 
			$condition = { $NOTEQ = { /Turn/now /State/person_stream } } 
			$then = { 
				$assign = { $local.select value =  { $rand = { 1 3 } } }
			}
		}
		
		$if = {
			$condition = { $NOTEQ = { $local.select $rand = { 1 3 } } } // fail 
			$then = { 
				$call = { id = 4013 i= 0 n = /Info/ONE_CARD_FAIL_CARD_NUM }
				# chk error?? - todo.. 
			}
		}
		$else = { # oneca success
			$then = {
				# end of change sha
				$if = {
					$condition = { $EQ = { /State/mode /Mode/CHANGE_SHA_MODE } }
					$then = {
						$assign = { /State/mode value = { /Mode/GENERAL_MODE } }
					}
				}
				# card put
				$insert2 = { 
					dir = { /CardList } 
					value = { /PutCard/cardId }
				}
				# mix
				$call  = { id = 7 }
	
				#  findindex( card )
				$findIndex = { 
					dir = { $concat = { /PlayerCardList $concat = { / $add = { /Turn/now -1 } } } }
					value = { $parameter.card } 
				}
				$assign = { $local.idx value = { $return_value = { } } }
					#putcard = getcard(idx);
				$assign = { /PutCard/cardId value = { $element = { $concat = { /PlayerCardList $concat = { / $add = { /Turn/now -1 } } } $local.idx } } }
				$remove3 = {
					dir = { $concat = { /PlayerCardList $concat = { / $add = { /Turn/now -1 } } } }
					value = { $local.idx }
				}
	
				# end of change sha
				$if = {
					$condition = { $EQ = { /State/mode /Mode/CHANGE_SHA_MODE } }
					$then = {
						$assign = { /State/mode value = { /Mode/GENERAL_MODE } }
					}
				}
	
				# card effect.
				$call = { id = 1008 card = $parameter.card }
			}
		}
	}
	Event = 
	{
		id = 4010
	
		$parameter = { i n }
		$local = { index card }
	
		$if = { $condition = { $COMP< = { $parameter.i $parameter.n } } 
			$then = {
				$assign = { $local.index  value = { $rand = { 0 $add = { -1 $size = { $concat = { 
					/PlayerCardList $concat = { / $add = { /Turn/now -1 } }
				  } } } } } }
				$assign = { $local.card value = { $element = { $concat = { /PlayerCardList $concat = { / $add = { /Turn/now -1 } } 
				} $local.index } } }
	
				$call = { id = 1006 put_card = /PutCard/cardId card = $local.card mode = /State/mode }
				
				$if = { 
					$condition = { $EQ = { TRUE $return_value = { } } }
					$then = {
						$assign = { /State/action_card value = { $local.card } }
						$assign = { /State/action_state value = { PUT } }
						$return = { $local.card }
					}
				}
				$call = { id = 4010 i = { $add = { $parameter.i 1 } } n = { $parameter.n } }
				##$assign = { /State/action_state value = { EAT } } # chk
				$return = { $return_value = { } }
			}
		}
	}
	
	
	# Main
	Main =
	{
		$call = { id = 0 }
	
		# $on = { id = ERROR }
		# $on = { id = CONSOLE }
	}

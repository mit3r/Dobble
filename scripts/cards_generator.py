#!/usr/bin/env python3

cards: list[list[int]] = []

n = 7 #Number of symbols per card - 1

#Add first set of n+1 cards (e.g. 8 cards)
for i in range(n+1):  
  #Add new card with first symbol
  cards.append([0])
  #Add n+1 symbols on the card (e.g. 8 symbols)
  for j in range(n):
    cards[i].append((j+1)+(i*n))

#Add n sets of n cards 
for i in range(0,n):
  for j in range(0,n):
    #Append a new card with 1 symbol
    cards.append([i+1])
     #Add n symbols on the card (e.g. 7 symbols)
    for k in range(0,n):
      val  = (n+1 + n*k + (i*k+j)%n)
      cards[len(cards)-1].append(val)


print("const int cards = {")
last = len(cards) - 1
for i, card in enumerate(cards):  
  print("    {" + ', '.join(map(str, card)) + " }" , end="\n" if i == last else ", \n")
print("};")

print(f'\nGenerated {n**2 + n + 1} cards')
print(f'Each card has {n + 1} symbols')
print(f'Total number of symbols: {n**2 + n + 1}')
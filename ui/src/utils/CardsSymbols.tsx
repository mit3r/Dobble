import cards from "@/assets/cards.json";
import icons from "@/assets/brainroots.json";

export type Card = number[];

export function getIconsSource(iconId: number): string {
  const card = icons.srcs[iconId];
  return `/brainroots/${card}`;
}

export function getCardSymbols(cardId: number): number[] {
  return cards.cards[cardId];
}

/**
 * Generates a quasi-random rotation angle for an icon on a card.
 * @returns A rotation angle in degrees [0; 360)
 */
export function quasiRandomRotation(cardId: number, iconIndex: number): number {
  const prime = 7;
  return (cardId * prime + iconIndex * prime * prime) % 360;
}

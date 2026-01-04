import type { PlayerGameInfo } from "@/types/dobble";

export default function YourStats({ you }: { you: PlayerGameInfo }) {
  return (
    <div className="grid gap-2 grid-cols-2 p-4 border rounded-lg bg-white/10 text-right">
      <h3 className="font-bold">Your Stats</h3>
      <p>Score: {you.points}</p>
      <p>Matches: {you.matches}</p>
      <p>Mistakes: {you.mistakes}</p>
    </div>
  );
}

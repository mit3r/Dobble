import type { PlayerGameInfo } from "@/types/dobble";

export default function YourStats({ you }: { you: PlayerGameInfo }) {
  return (
    <div className="panel">
      <h3 className="font-bold text-lg mb-3 text-dobble-text">
        Your Stats
      </h3>
      
      <div className="grid grid-cols-3 gap-4 text-center">
        <div className="p-3 rounded-xl bg-dobble-card">
          <p className="text-2xl font-bold text-dobble-success">{you.points}</p>
          <p className="text-xs text-dobble-text-muted">Points</p>
        </div>
        <div className="p-3 rounded-xl bg-dobble-card">
          <p className="text-2xl font-bold text-dobble-primary">{you.matches}</p>
          <p className="text-xs text-dobble-text-muted">Matches</p>
        </div>
        <div className="p-3 rounded-xl bg-dobble-card">
          <p className="text-2xl font-bold text-dobble-danger">{you.mistakes}</p>
          <p className="text-xs text-dobble-text-muted">Mistakes</p>
        </div>
      </div>
    </div>
  );
}

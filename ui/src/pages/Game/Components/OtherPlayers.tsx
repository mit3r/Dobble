import type { PlayerGameInfo } from "@/types/dobble";
import { clsx } from "clsx";
import CardComponent from "./CardComponent";

export default function OtherPlayers(props: { observer: boolean; playersInfo: PlayerGameInfo[] }) {
  return (
    <div className="panel flex flex-col overflow-hidden h-full">
      <h2 className="font-bold text-lg mb-4 text-dobble-text flex items-center gap-2">
        Other Players
        <span className="badge badge-online text-xs">{props.playersInfo.length}</span>
      </h2>

      <div className="flex-1 grid grid-cols-1 2xl:grid-cols-2 min-h-32 auto-rows-min overflow-y-auto overflow-x-hidden custom-scrollbar gap-2">
        {props.playersInfo.map((player, index) => (
          <OtherPlayerComponent 
            key={player.nickname} 
            player={player} 
            extended={props.observer}
            rank={index + 1}
          />
        ))}
      </div>
    </div>
  );
}

function OtherPlayerComponent(props: { player: PlayerGameInfo; extended?: boolean; rank: number }) {
  return (
    <div
      className={clsx(
        "overflow-clip flex justify-between gap-3 p-3 rounded-xl bg-dobble-card hover:bg-dobble-card-hover transition-colors",
        props.extended ? "h-40" : "h-32"
      )}
    >
      <div className="flex flex-col h-full shrink-0 justify-between">
        <div>
          <div className="flex items-center gap-2">
            <span className="text-dobble-accent font-bold">#{props.rank}</span>
            <h3 className="text-lg font-bold text-dobble-text truncate">{props.player.nickname}</h3>
          </div>
        </div>
        
        <div className="space-y-1 text-sm">
          <p className="flex justify-between gap-4">
            <span className="text-dobble-text-muted">Points:</span>
            <span className="font-bold text-dobble-success">{props.player.points}</span>
          </p>
          <p className="flex justify-between gap-4">
            <span className="text-dobble-text-muted">Matches:</span>
            <span className="font-bold">{props.player.matches}</span>
          </p>
          <p className="flex justify-between gap-4">
            <span className="text-dobble-text-muted">Mistakes:</span>
            <span className="font-bold text-dobble-danger">{props.player.mistakes}</span>
          </p>
        </div>
      </div>

      <div className="h-full flex justify-end">
        <CardComponent cardId={props.player.cardId} disabled={true} />
      </div>
    </div>
  );
}

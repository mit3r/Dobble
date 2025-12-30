import Dots from "@/Components/Dots";
import { useStore } from "zustand";
import { mainStore } from "../../store";

export default function LobbyPage() {
  const lobbyInfo = useStore(mainStore, (state) => state.game.gameInfo);

  // const lobbyInfo: GameInfo = {
  //   gameId: "ABCD1234",
  //   gameName: "Alpha Squad",
  //   players: [
  //     {
  //       nickname: "Player1",
  //       cardId: 1,
  //       score: 10,
  //       badMatches: 2,
  //       goodMatches: 5,
  //     },
  //     {
  //       nickname: "Player2",
  //       cardId: 2,
  //       score: 15,
  //       badMatches: 1,
  //       goodMatches: 7,
  //     },
  //     {
  //       nickname: "Player3",
  //       cardId: 2,
  //       score: 15,
  //       badMatches: 1,
  //       goodMatches: 7,
  //     },
  //   ],
  //   lastsCards: 0,
  //   topCardId: 0,
  //   status: GameStatus.Waiting,
  // };

  return (
    <div className="h-full flex flex-col justify-end gap-4">
      {lobbyInfo === null ? (
        <h1 className="flex items-center justify-center h-full w-full">
          Loading lobby
          <Dots />
        </h1>
      ) : (
        <div className="flex flex-col h-full">
          <div className="flex items-center justify-center relative h-fit py-4">
            <h2 className="text-xl absolute left-0">Room {lobbyInfo.gameName}</h2>
            <h1 className="text-4xl font-bold">Dobble</h1>
            <h2 className="text-xl absolute right-0">Waiting</h2>
          </div>

          <div className="flex flex-col flex-1 min-h-0">
            <div className="p-2  text-xl">Players</div>
            <ol className="overflow-y-auto flex-1 border-y-2">
              {lobbyInfo.players.map((player) => (
                <li key={player.nickname} className="flex justify-between p-2 border-b">
                  <span>{player.nickname}</span>
                </li>
              ))}
            </ol>
          </div>
        </div>
      )}

      <div className="w-full flex justify-end items-center">
        <button className="p-2 px-8 border-2 font-bold">Quit</button>
      </div>
    </div>
  );
}

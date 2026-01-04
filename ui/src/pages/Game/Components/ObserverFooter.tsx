export default function ObserverFooter() {
  return (
    <div className="w-full grid grid-cols-[1fr_auto] gap-4 border-t-2 p-4">
      <div className="flex justify-center items-center">
        <span className="text-2xl">You are observing the game.</span>
      </div>

      <button className="p-2 px-4 border-2" onClick={() => window.bridges?.game.callQuitGame()}>
        Quit Game
      </button>
    </div>
  );
}

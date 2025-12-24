import { useStore } from "zustand";
import { mainStore } from "../../store";

export default function StartPage() {
  const setPage = useStore(mainStore, (state) => state.setPage);

  return (
    <div>
      <h1>Dobble</h1>

      <p>
        Welcome to Dobble, the exciting card game where players test their observation skills and
        reflexes! The objective of the game is to be the first to spot the matching symbol between
        two cards. Each card contains a variety of symbols, but only one symbol is common between
        any two cards.
      </p>

      <ol>
        <li>
          <strong>Room 1</strong>

          <span>1/4</span>
          <span>waiting</span>
        </li>
      </ol>

      <button onClick={() => setPage("room")}>Join Room</button>
    </div>
  );
}

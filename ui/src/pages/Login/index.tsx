import LobbyServStatus from "@/Components/ServerStatus/LobbyServStatus";
import { mainStore } from "@/store";
import { useStore } from "zustand";
import NicknameComponent from "./Components/NicknameComponent";
import AddressComponent from "./Components/AddressComponent";

export default function LoginPage() {
  const lobbyAddress = useStore(mainStore, (s) => s.browser.lobbyAddress);

  return (
    <div className="h-full flex flex-col items-center">
      {lobbyAddress && <LobbyServStatus />}

      <div className="flex flex-col flex-1 gap-8 p-8">
        <div className="text-center">
          <h1 className="text-4xl font-bold mb-4">Dobble</h1>
          <p className="text-2xl">Welcome to Dobble game!</p>
        </div>
        {lobbyAddress ? <NicknameComponent /> : <AddressComponent />}
      </div>
    </div>
  );
}

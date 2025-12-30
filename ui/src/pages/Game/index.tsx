import CardComponent from "./Components/CardComponent";

export default function GamePage() {
  // const topCardId = 0;

  const cards = [12, 45, 28, 23, 56, 34, 5, 47, 19];

  return (
    <div>
      Game Page
      <div className="w-full grid grid-cols-3 gap-4">
        {cards.map((cardId, index) => (
          <div key={index} className="h-full m-2">
            <CardComponent cardId={cardId} />
          </div>
        ))}
      </div>
    </div>
  );
}

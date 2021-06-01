public class Bishop extends Piece {

	public Bishop(int col){
		color = col;
		name = BISHOP;
		nameS = "Bishop";
		
		if(players == 1) team = color;
		
		else {
			if(color == BLACK) 	team = 1;
			else team = 2;
		}
	}
	
	public ArrayList<Position> getMovement(Board_Master board, Position now) {
		
		
		int x = getX();
		int y = getY();
		
		ArrayList<Position> go = new ArrayList<Position>();
		
		for(int i = 1, j = 1; Position_InRange(x + i, y + j); i++, j++)
		{
			int goX = x + i;
			int goY = y + j;
			
			if(getPiece(goX, goY) == 0)
				go_add(new Position(goX, goY));
			
			else
			{
				if(getPiece(goX, goY) != team)
					go_add(new Position(goX, goY));			
				break;
			}
		}
		
		for(int i = 1, j = -1; inRange(x + i, y + j); i++, j--)
		{
			int goX = x + i;
			int goY = y + j;
			
			if(getPiece(goX, goY) == 0)
				go_add(new Position(goX, goY));
			
			else
			{
				if(getPiece(goX, goY) != team)
					go_add(new Position(goX, goY));			
				break;
			}
		}
		
		for(int i = -1, j = 1; Position_inRange(x + i, y + j); i--, j++)
		{
			int goX = x + i;
			int goY = y + j;
			
			if(getPiece(goX, goY) == 0)
				go_add(new Position(goX, goY));
			
			else
			{
				if(getPiece(goX, goY) != team)
					go_add(new Position(goX, goY));			
				break;
			}
		}
		
		for(int i = -1, j = -1; inRange(x + i, y + j); i--, j--)
		{
			int goX = x + i;
			int goY = y + j;
			
			if(getPiece(goX, goY) == 0)
				go_add(new Position(goX, goY));
			
			else
			{
				if(getPiece(goX, goY) != team)
					go_add(new Position(goX, goY));			
				break;
			}
		}
		
		return go;
	}

}
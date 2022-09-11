#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME IA_Misteri


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  typedef vector<vector<char>> Matriu;
  typedef vector<vector<bool>> Matriu_visitat;
  const vector<Dir> dirs = {Up,Down,Left,Right};
  

  void sub_mat(Matriu& mat, int n, int m){ //creo una matriu q es el contingut del tauler en chars
       
    for(int i = 0; i < n; ++i){
      for(int j = 0; j < m; ++j){
        CellType c = cell(i, j).type;
        switch(c){
          case Street: mat[i][j] = '.'; break;
          case Building: mat[i][j] = 'B'; break;
          default: break;
        }

        WeaponType w = cell(i, j).weapon;
        switch (w){
          case Gun: mat[i][j] = 'G'; break;
          case Bazooka: mat[i][j] = 'Z'; break;
          default: break;
        }

        BonusType b = cell(i, j).bonus;
        switch(b){
          case Money: mat[i][j] = 'M'; break;
          case Food: mat[i][j] = 'F'; break;
          default: break;
        }
        
        int r = cell(i, j).id;
        if (r != -1) mat[i][j] = 'C';
      }
    }


  }



  Dir mata_bfs(Pos p){//nomes necessito la meva posicio
    int n = board_rows();
    int m =  board_cols();
   
    Matriu_visitat vist(n,vector<bool>(m,false));
 
    
   
    queue<Dir> q;
    queue<Pos> pq;
  //mirar las 4 posiciones adyacentes
    for(int j = 0; j < 4; ++j){
      Pos newpos = p + dirs[j];
      //cell(newpos).type == Street
      if(pos_ok(newpos)){
	      Cell ce = cell(newpos);
	      if(ce.type != Building and vist[newpos.i][newpos.j] == false){
	      	vist[newpos.i][newpos.j] = true;
	       
	        if(ce.id != -1){
	        	if(citizen(cell(newpos).id).player != me()) return dirs[j];
	        }
          else if (ce.bonus == Money or ce.bonus == Food or ce.weapon == Bazooka or ce.weapon == Gun) {
            return dirs[j];
          }
	        else {
	          pq.push(newpos);
	          q.push(dirs[j]);
	        }
	    }
      }
    }
    
    while(not pq.empty()) {
      
        Pos act = pq.front();
        pq.pop();
        Dir d_act = q.front();
        q.pop(); //direccion que ha de seguir el jugador desde su posicion
        for(int j = 0; j < 4; ++j){
         
          Pos newpos = act + dirs[j];
          if(pos_ok(newpos)){
	          Cell ce = cell(newpos);
	          if(pos_ok(newpos) and ce.type != Building and vist[newpos.i][newpos.j] == false) {

	            
		        if(ce.id != -1){
		        	if(citizen(cell(newpos).id).player != me()) return dirs[j];
		        }

	            vist[newpos.i][newpos.j] = true;
	            pq.push(newpos);
	            q.push(d_act);
	          }
	      }
          
        }
      
    }
    
   return Up;
  }

  //bool de si hi ha
  bool mira_bfs(Pos p, char c){//p posicio original, c caracter q busco
    int n = board_rows();
    int m =  board_cols();
   
    Matriu taulell(n,vector<char>(m));
    sub_mat(taulell, n,m);
    
    //if (round() == num_rounds()-1) escriu_matriu(taulell, n, m);
    queue<Dir> q;
    queue<Pos> pq;
  //mirar las 4 posiciones adyacentes
    for(int j = 0; j < 4; ++j){
      Pos newpos = p + dirs[j];
      //cell(newpos).type == Street
      if(pos_ok(newpos) and taulell[newpos.i][newpos.j] != 'B' and taulell[newpos.i][newpos.j] != 'C' and (cell(newpos).b_owner == -1 or (cell(newpos).b_owner != -1 and cell(newpos).b_owner == me()))  ){
       
        if(taulell[newpos.i][newpos.j] == c){
          return true;
        }
        else {
          pq.push(newpos);
          q.push(dirs[j]);
        }
      }
    }
    
    while(not pq.empty()) {
      
        Pos act = pq.front();
        pq.pop();
        Dir d_act = q.front();
        q.pop(); //direccion que ha de seguir el jugador desde su posicion
        for(int j = 0; j < 4; ++j){
         
          Pos newpos = act + dirs[j];
          if(pos_ok(newpos) and taulell[newpos.i][newpos.j] != 'B' and taulell[newpos.i][newpos.j] != 'C') {
            if(taulell[newpos.i][newpos.j] == c){
              return true;
            }
            taulell[newpos.i][newpos.j] = 'B';
            pq.push(newpos);
            q.push(d_act);
          }
          
        }
      
    }
    
   return false;
  }


  //anar a buscar objectiu
  Dir go_bfs(Pos p,char c){//p posicio original, c caracter q busco
    int n = board_rows();
    int m =  board_cols();
   
    Matriu taulell(n,vector<char>(m));
    sub_mat(taulell, n,m);
    
    //if (round() == num_rounds()-1) escriu_matriu(taulell, n, m);
    queue<Dir> q;
    queue<Pos> pq;
  //mirar las 4 posiciones adyacentes
    for(int j = 0; j < 4; ++j){
      Pos newpos = p + dirs[j];
      //cell(newpos).type == Street
      if(pos_ok(newpos) and taulell[newpos.i][newpos.j] != 'B' and taulell[newpos.i][newpos.j] != 'C' and (cell(newpos).b_owner == -1 or (cell(newpos).b_owner != -1 and cell(newpos).b_owner == me()))  ){
       
        if(taulell[newpos.i][newpos.j] == c){
          return dirs[j];
        }
        else if (taulell[newpos.i][newpos.j] == 'M' or taulell[newpos.i][newpos.j] == 'F' or taulell[newpos.i][newpos.j] == 'Z' or taulell[newpos.i][newpos.j] == 'G') {
            return dirs[j];
          }
        else {
          pq.push(newpos);
          q.push(dirs[j]);
        }
      }
    }
    
    while(not pq.empty()) {
      
        Pos act = pq.front();
        pq.pop();
        Dir d_act = q.front();
        q.pop(); //direccion que ha de seguir el jugador desde su posicion
        for(int j = 0; j < 4; ++j){
         
          Pos newpos = act + dirs[j];
          if(pos_ok(newpos) and taulell[newpos.i][newpos.j] != 'B' and taulell[newpos.i][newpos.j] != 'C') {
            if(taulell[newpos.i][newpos.j] == c){
              return d_act;
            }
            taulell[newpos.i][newpos.j] = 'B';
            pq.push(newpos);
            q.push(d_act);
          }
          
        }
      
    }
    
   return Up;
  }
  

  /**
   * Play method, invoked once per each round.
   */
virtual void play () {
  vector<int> w = warriors(me());
  vector<int> b = builders(me());
  bool alguien_vivo = false;
  for (int id : w) {
    if (citizen(id).life > 0) alguien_vivo = true;
  }

  if(is_day()){// dia
    for (int id : w) {// prioritas warriors
      Pos p = citizen(id).pos;
      if(citizen(id).weapon != Bazooka) {
        if (mira_bfs(p,'Z')){
          char c = 'Z'; //busco bazuka
          Dir voltant = go_bfs(p,c);
        
          int ciutada = citizen(id).id;
          move(ciutada,voltant);
        }
      	else if(citizen(id).weapon !=  Gun and mira_bfs(p,'G')){
          char c = 'G';
          Dir voltant = go_bfs(p,c);
          int ciutada = citizen(id).id;
          move(ciutada,voltant);
        }
        //busca bonus
        else if(citizen(id).life != warrior_ini_life()){//low vida
          char c = 'F';
          Dir voltant = go_bfs(p,c); //busca menjar
          int ciutada = citizen(id).id;
          move(ciutada,voltant);
        }
        else{
          char c = 'M';
          Dir voltant = go_bfs(p,c); //a por monedicas
          int ciutada = citizen(id).id;
          move(ciutada,voltant);
        }
      }
      else {
        if(citizen(id).life != warrior_ini_life()){//low vida
          char c = 'F';
          Dir voltant = go_bfs(p,c); //busca menjar
          int ciutada = citizen(id).id;
          move(ciutada,voltant);
        }
        else{
          char c = 'M';
          Dir voltant = go_bfs(p,c); //a por monedicas
          int ciutada = citizen(id).id;
          move(ciutada,voltant);
        }
      }
    }
  }
  else if(is_night()){//nit
    for (int id : w) {// prioritas warriors
      Pos p = citizen(id).pos;
      
      if(citizen(id).weapon != Bazooka) {
        if(mira_bfs(p,'Z')){
      		char c = 'Z'; //busco bazuka
     
      		Dir voltant = go_bfs(p,c);
      
      		int ciutada = citizen(id).id;
      		move(ciutada,voltant);
      	}
        else if(citizen(id).weapon !=  Gun and mira_bfs(p,'G')){
          char c = 'G';
          Dir voltant = go_bfs(p,c);
          int ciutada = citizen(id).id;
          move(ciutada,voltant);
        }    	
        else{
        
          Dir voltant = mata_bfs(p);
          int ciutada = citizen(id).id;
          move(ciutada,voltant);
        }
      }
      else {
        Dir voltant = mata_bfs(p);
        int ciutada = citizen(id).id;
        move(ciutada,voltant);
      }		
    }
  }
  for(int id : b){//prioritats builder
    Pos p = citizen(id).pos;
    if (not alguien_vivo) {
      if(mira_bfs(p,'Z')){ //robar bazuka 
        char c = 'Z'; //busco bazuka
     
      	Dir voltant = go_bfs(p,c);
      
      	int ciutada = citizen(id).id;
      	move(ciutada,voltant);

      }
      else if (mira_bfs(p,'G')){ //robar bazuka 
        char c = 'G'; //busco bazuka
     
      	Dir voltant = go_bfs(p,c);
      
      	int ciutada = citizen(id).id;
      	move(ciutada,voltant);
      }
      else if(citizen(id).life != builder_ini_life()){//low vida
        char c = 'F';
        Dir voltant = go_bfs(p,c); //busca menjar
        int ciutada = citizen(id).id;
        move(ciutada,voltant);
      }
      else{
        char c = 'M';
        Dir voltant = go_bfs(p,c); //a por monedicas
        int ciutada = citizen(id).id;
        move(ciutada,voltant);

      }
    }
    else {
      if(citizen(id).life != builder_ini_life()){//low vida
        char c = 'F';
        Dir voltant = go_bfs(p,c); //busca menjar
        int ciutada = citizen(id).id;
        move(ciutada,voltant);
      }
      else{
        char c = 'M';
        Dir voltant = go_bfs(p,c); //a por monedicas
        int ciutada = citizen(id).id;
        move(ciutada,voltant);
      }
    }
  }
}
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

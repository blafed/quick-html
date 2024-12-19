interface Binding {

}


interface Element { }
interface Canvas { linewidth: number }


let c: Canvas = {
    get linewidth() { return 0; },
    set linewidth(v: number) { }

};
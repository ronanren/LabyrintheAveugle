#let conf(
  titre: none,
  auteurs: (),
  module: none,
  promotion: none,
  annee: 0,
  doc,
) = {
  set text(lang:"fr")

//  _____   ____    ____    ____    _____ 
// /  __/  /  _ \  /  __\  /  _ \  /  __/ 
// | |  _  | / \|  |  \/|  | | \|  |  \   
// | |_//  | |-||  |    /  | |_/|  |  /_  
// \____\  \_/ \|  \_/\_\  \____/  \____\ 
//                                        
let ncols = calc.min(auteurs.len(), 4)
grid(
  columns: (1fr,) * ncols,
  row-gutter: 24pt,
  ..auteurs.map(auteur => [
    #auteur.nom \
    #link("mailto:" + auteur.email)
  ]),
)
grid(
  columns: (1fr, 1fr),
  row-gutter: 12pt,
  text(17pt, "Module : "),
  text(17pt, "Promotion : "),
  text(17pt, module),
  text(17pt, promotion),
)
set align(center)
v(200pt)
text(30pt, titre)
v(250pt)
scale(x:40%, y:40%, image("Enssat-UnivRennes_RVB.png"))

//  ____    ____    ____    _       _       _____   _       _____ 
// /  _ \  /  _ \  /   _\  / \ /\  / \__/| /  __/  / \  /| /__ __\
// | | \|  | / \|  |  /    | | ||  | |\/|| |  \    | |\ ||   / \  
// | |_/|  | \_/|  |  \_   | \_/|  | |  || |  /_   | | \||   | |  
// \____/  \____/  \____/  \____/  \_/  \| \____\  \_/  \|   \_/  
//                                                                
  pagebreak()
  set align(left)
  set page(
    paper: "a4",
    numbering: "1", // page numbering
    number-align: center,
    header: grid(
      columns: (1fr, 1fr),
      grid(
        row-gutter: 5pt,
        ..auteurs.map(auteur => [ #auteur.nom ]),
      ),
      titre,
    ),
  )
  set text(size: 11pt)
  set par(justify: true)
  set heading(numbering: "1.a.i")
  set list(marker: [--])
  show link: underline
  doc
}

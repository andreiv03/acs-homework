module Deep where

import Shallow (Point, Region, Transformation)
import qualified Shallow as S

{-
  Deep embeddings pentru regiuni și transformări. Fiecare regiune
  și transformare este reprezentată sub forma unui arbore sintactic
  (abstract syntax tree, AST) ce descrie secvența de operații care contribuie
  la construcția acelei regiuni sau transformări. De exemplu, expresia
  (circles 2), unde circles a fost definită în etapa 1, ar produce acum
  un rezultat similar cu

  Union (Circle 2.0) (Transform (Translation 6.0 0.0) (Circle 2.0)).

  Pentru a obține acest efect, toate funcțiile din etapa 1 sunt reimplementate
  astfel încât să utilizeze direct constructorul de date potrivit al unui
  tip de date. De exemplu, funcția fromPoints *este* acum constructorul
  FromPoints.

  Primul avantaj major al reprezentării bazate pe AST-uri este posibilitatea
  interpretării acesteia în diverse moduri pentru a reconstitui semnificații
  concrete variate ale regiunilor și transformărilor, e.g. regiuni ca funcții
  caracteristice, și transformări ca funcții pe puncte, ca în etapa 1.
  Vom vedea și alte semnificații concrete în etapa 3.

  Al doilea mare avantaj îl constituie posibilitatea simplificării AST-ului
  înaintea interpretării lui într-o manieră specifică. Observați deja cum
  funcțiile combineTransformations și applyTransformation de mai jos, văzute
  ca smart constructors, recunosc anumite cazuri particulare și simplifică
  AST-ul încă de la construcție.
-}
data RegionAST
  = FromPoints [Point]
  | Rectangle Float Float
  | Circle Float
  | Complement RegionAST
  | Union RegionAST RegionAST
  | Intersection RegionAST RegionAST
  | Transform TransformationAST RegionAST
  deriving (Show, Eq)

data TransformationAST
  = Translation Float Float
  | Scaling Float
  | Combine [TransformationAST]
  deriving (Show, Eq)

fromPoints :: [Point] -> RegionAST
fromPoints = FromPoints

rectangle :: Float -> Float -> RegionAST
rectangle = Rectangle

circle :: Float -> RegionAST
circle = Circle

complement :: RegionAST -> RegionAST
complement = Complement

union :: RegionAST -> RegionAST -> RegionAST
union = Union

intersection :: RegionAST -> RegionAST -> RegionAST
intersection = Intersection

translation :: Float -> Float -> TransformationAST
translation = Translation

scaling :: Float -> TransformationAST
scaling = Scaling

{-
  Smart constructor: dacă lista de transformări este singleton, înlocuiește
  lista cu unica transformare din listă; altfel, utilizează constructorul
  de date Combine.
-}
combineTransformations :: [TransformationAST] -> TransformationAST
combineTransformations [transformation] = transformation
combineTransformations transformations = Combine transformations

{-
  Smart constructor: dacă se dorește aplicarea unei liste vide de transformări
  asupra unei regiuni, întoarce regiunea ca atare; altfel, utilizează
  constructorul de date Transform.
-}
applyTransformation :: TransformationAST -> RegionAST -> RegionAST
applyTransformation (Combine []) region = region
applyTransformation transformation region = Transform transformation region

{-
  *** TODO ***

  Implementați funcția toTransformation, care constituie o interpretare
  a AST-ului unei transformări (TransformationAST), în vederea recuperării
  reprezentării concrete din etapa 1, sub forma unei funcții cu tipul
  Transformation = (Point -> Point).

  Nu este necesar să implementați de la zero, ci puteți invoca direct
  funcțiile din etapa 1, prefixându-le cu "S." (observați la începutul acestui
  fișier linia "import qualified Shallow as S"). Mai precis, funcțiile
  neprefixate, e.g. translation, sunt cele din acest fișier, iar funcțiile
  prefixate, e.g. S.translation, sunt cele din modulul Shallow.
-}
toTransformation :: TransformationAST -> Transformation
toTransformation transformation = toTransformation transformation
  where
    toTransformation (Translation x y) = S.translation x y
    toTransformation (Scaling x) = S.scaling x
    toTransformation (Combine transformations) = S.combineTransformations (map toTransformation transformations)

{-
  *** TODO ***

  Implementați funcția toRegion, care constituie o interpretare a AST-ului
  unei regiuni (RegionAST), în vederea recuperării reprezentării concrete
  din etapa 1, sub forma unei funcții caracteristice cu tipul
  Region = (Point -> Bool).
-}
toRegion :: RegionAST -> Region
toRegion region = toRegion region
  where
    toRegion (FromPoints points) = S.fromPoints points
    toRegion (Rectangle x y) = S.rectangle x y
    toRegion (Circle r) = S.circle r
    toRegion (Complement region) = S.complement (toRegion region)
    toRegion (Union region1 region2) = S.union (toRegion region1) (toRegion region2)
    toRegion (Intersection region1 region2) = S.intersection (toRegion region1) (toRegion region2)
    toRegion (Transform transformation region) = S.applyTransformation (toTransformation transformation) (toRegion region)

{-
  Varianta actualizată a a funcției inside.
-}
inside :: Point -> RegionAST -> Bool
inside = flip toRegion

{-
  *** TODO ***

  Implementați funcția decomposeTransformation, care descompune o transformare
  oricât de complexă într-o listă de transformări elementare (translații
  și scalări), conservând bineînțeles ordinea acestora.

  Constrângeri: evitați recursivitatea explicită.

  Hint: valorificați ordinea de sus în jos de realizare a pattern matching-ului,
  pentru a scurta descrierea cazurilor.

  Exemple:

  > decomposeTransformation $ Translation 1 2
  [Translation 1.0 2.0]

  > decomposeTransformation $ Scaling 2
  [Scaling 2.0]

  > decomposeTransformation $
      Combine [ Translation 1 2
              , Combine [ Translation 3 4
                        , Scaling 2
                        ]
              , Scaling 3
              ]
  [Translation 1.0 2.0,Translation 3.0 4.0,Scaling 2.0,Scaling 3.0]
-}
decomposeTransformation :: TransformationAST -> [TransformationAST]
decomposeTransformation transformation = decomposeTransformation transformation
  where
    decomposeTransformation (Translation x y) = [Translation x y]
    decomposeTransformation (Scaling x) = [Scaling x]
    decomposeTransformation (Combine transformations) = concatMap decomposeTransformation transformations

{-
  *** TODO ***

  Implementați funcția fuseTransformations, care alipește transformările
  adiacente de același fel (translații cu translații și scalări cu scalări)
  dintr-o listă de transformări elementare (translații și scalări),
  și întoarce lista transformărilor rezultante.

  Constrângeri: evitați recursivitatea explicită.

  > fuseTransformations [Translation 1 2]
  [Translation 1.0 2.0]

  > fuseTransformations [Scaling 2, Scaling 3]             
  [Scaling 6.0]

  > fuseTransformations [ Translation 1 2, Translation 3 4
                        , Scaling 2, Scaling 3
                        , Translation 5 6
                        ]
  [Translation 4.0 6.0,Scaling 6.0,Translation 5.0 6.0]
-}
fuseTransformations :: [TransformationAST] -> [TransformationAST]
fuseTransformations = foldr fuseTransformations []
  where
    fuseTransformations (Translation x1 y1) (Translation x2 y2 : list) = Translation (x1 + x2) (y1 + y2) : list
    fuseTransformations (Scaling x1) (Scaling x2 : list) = Scaling (x1 * x2) : list
    fuseTransformations transformation list = transformation : list

{-
  *** TODO ***

  Implementați funcția optimizeTransformations, care simplifică toate
  transformările din AST-ul unei regiuni. Principiile sunt următoarele:

  * Transformările succesive trebuie descompuse și alipite.
  * Pentru a evidenția lanțuri cât mai lungi de transformări succesive,
    se urmărește deplasarea în sus a transformărilor din AST, astfel:
    * Complementul unei transformări este transformarea complementului.
    * O ramificare (reuniune sau intersecție) de transformări de regiuni
      presupune determinarea celui mai lung prefix de transformări comune
      de pe cele două ramuri și deplasarea acestuia deasupra ramificării,
      păstrând sub ea sufixele de transformări necomune.
  * O regiune elementară (din puncte, dreptunghi sau cerc) se consideră
    optimizată.
  * Toate cosmetizările de mai sus se realizează după optimizarea recursivă
    a subregiunilor.
  
  Constrângeri: evitați duplicarea codului.

  Hints:

  * case pentru pattern matching în interiorul altor expresii
  * smart constructors: combineTransformation, applyTransformation

  Exemple:

  > optimizeTransformations $
      Transform (Combine [ Translation 1 2
                          , Combine [ Translation 3 4
                                    , Scaling 2
                                    ]  
                          , Scaling 3
                          ])
                (Circle 5)
  Transform (Combine [Translation 4.0 6.0,Scaling 6.0]) (Circle 5.0)

  > optimizeTransformations $
      Transform (Combine [ Translation 1 2
                          , Combine [ Translation 3 4
                                    , Scaling 2
                                    ]  
                          , Scaling 3
                          ])
                (Transform (Scaling 4)
                            (Transform (Scaling 2) (Circle 5)))
  Transform (Combine [Translation 4.0 6.0,Scaling 48.0]) (Circle 5.0)

  > optimizeTransformations $
      Complement (Transform (Scaling 4)
                            (Transform (Scaling 2) (Circle 5)))
  Transform (Scaling 8.0) (Complement (Circle 5.0))

  > optimizeTransformations $
      Transform (Combine [ Translation 1 2
                          , Combine [ Translation 3 4
                                    , Scaling 2
                                    ]  
                          , Scaling 3
                          ])
                (Complement (Transform (Scaling 4)
                                        (Transform (Scaling 2) (Circle 5))))
  Transform (Combine [Translation 4.0 6.0,Scaling 48.0]) (Complement (Circle 5.0))

  > optimizeTransformations $
      Union (Complement (Transform (Scaling 4)
                                    (Transform (Scaling 2) (Circle 5))))
            (Rectangle 6 7)
  Union (Transform (Scaling 8.0) (Complement (Circle 5.0))) (Rectangle 6.0 7.0)

  > optimizeTransformations $
      Union (Transform (Combine [ Translation 1 2
                                , Combine [ Translation 3 4
                                          , Scaling 2
                                          ]  
                                , Scaling 3
                                ])
                        (Complement (Transform (Scaling 4)
                                              (Transform (Scaling 2) (Circle 5)))))
            (Transform (Translation 4 6) (Rectangle 6 7))
  Transform (Translation 4.0 6.0)
            (Union (Transform (Scaling 48.0) (Complement (Circle 5.0)))
                    (Rectangle 6.0 7.0))
-}
optimizeTransformations :: RegionAST -> RegionAST
optimizeTransformations region = optimizeTransformations region
  where
    optimizeTransformations (FromPoints points) = FromPoints points
    optimizeTransformations (Rectangle x y) = Rectangle x y
    optimizeTransformations (Circle r) = Circle r
    optimizeTransformations (Complement region) = Complement (optimizeTransformations region)
    optimizeTransformations (Union region1 region2) = Union (optimizeTransformations region1) (optimizeTransformations region2)
    optimizeTransformations (Intersection region1 region2) = Intersection (optimizeTransformations region1) (optimizeTransformations region2)
    optimizeTransformations (Transform transformation region) = applyTransformation (combineTransformations (fuseTransformations (decomposeTransformation transformation))) (optimizeTransformations region)
    
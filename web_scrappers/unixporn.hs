{- reads reddit and pulls out github links from r/unixporn -}

{-# lANGUAGE Arrows, NoMonomorphismRestriction #-}
import Text.XML.HXT.Core
import Network.HTTP
import Network.URI
import Data.List

reddit_src = "http://www.reddit.com/r/unixporn.xml"
github_pat = "github.com"

{- obtain html content from url -}
get url = do
  case parseURI url of
    Nothing   -> ioError . userError $ "Invalid URL"
    Just    u -> do
      res <- simpleHTTP (Request u GET [] "")
      case res of
        Left  _ -> ioError . userError $ "Failed to get " ++ show u
        Right r -> return $ rspBody r

{- produce XML tree from web content "doc" -}
parseXML :: String -> IOStateArrow s b XmlTree
parseXML doc = readString [ withValidate no, withRemoveWS yes] doc

{- useful utility functions -}
atTag     tag = deep (isElem >>> hasName tag)
text          = getChildren >>> getText
textAtTag tag = atTag tag >>> text

concatM :: Monad m => m [[a]] -> m [a]
concatM a = a >>= \x -> return (concat x)

appendM :: Monad m => [a] -> m [[a]] -> m [[a]]
appendM s m = m >>= \x -> return (map ((++) s) x)

{- pulls all post links from subreddit page -}
getPostLinks :: String -> IO [String]
getPostLinks src = do
  doc  <- get src
  xml  <- return $ parseXML doc
  runX (xml >>> getPosts >>> returnA)
  where
  getPosts = textAtTag "link" >>> isA (isInfixOf "comments") >>> arr (++ ".xml")

{- produce a list of github links from each link -}
getLinks :: String -> IO [String]
getLinks lnk = do
  doc <- get lnk
  xml <- return $ parseXML doc
  runX (xml >>> getRepos >>> returnA)
  where
  getRepos = textAtTag "description" >>> isA (isInfixOf github_pat) >>> arr filterURL
  filterURL = filterURL' False []
    where
    filterURL' False r [] = r
    filterURL' False r l@(x:xs) =
      if isPrefixOf github_pat l
      then filterURL' True (reverse github_pat) $ drop (length github_pat) l
      else filterURL' False r xs
    filterURL' True r [] = reverse r
    filterURL' True r (x:xs) =
      case x of
      ' '       -> reverse r
      ')'       -> reverse r
      otherwise -> filterURL' True (x:r) xs

{- special formatting -}
formatPrint :: [String] -> IO ()
formatPrint [] = return ()
formatPrint (x:xs) = do
  print x
  formatPrint xs

main = do
  lnks <- getPostLinks reddit_src 
  res  <- appendM "www." . concatM . mapM getLinks $ lnks
  formatPrint res

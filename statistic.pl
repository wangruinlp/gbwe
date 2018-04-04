# This is for counting the co-occurrences of words in a bilingual corpus.
# transformer .txt en acom


use strict;
use locale;
use strict;
use locale;
use utf8;
use open ':encoding(utf8)';
 binmode(STDOUT, ":utf8");
binmode(STDIN, ":encoding(utf8)");   


# @caracteres={" , « , » , ‹ , › , “ , ” , „ , ' , ‘ , ’ , ‚ , … , ! , ¡ , ¿ , ( , ) , [ , ] , { , } , ¨ , ´ , ` , ^ , ˆ , ~ , ˜ , ¸ , # , * , , , . , : , ; , · , • , ¯ , — , _ , | , ¦ , † , † , § , ¶ , © , ® , ™ , & , @ , / , \ ,  , µ , < , > , = , = , = , ˜ , = , ± , - , + , × , ÷ , / , % , ‰ , ¼ , ½ , ¾ , ¹ , ² , ³ , º , ª , ƒ , ' , ' , 8 , ¬  , ~ ,  Ø , * , · , < , > , a , ß ,  µ , };


my @lignec=();

my  %cooc;
my %freq;
 # assigns each line in turn to $_
while (my $ligne= <STDIN>) {
#chomp $ligne;
$ligne=lc($ligne);
my $lignecc=$ligne;
    # print "ici:", $ligne,"\n";
     
    while ($lignecc=~/[\n]/)#tant que la ligne contient une ponctuation
    {$_=$lignecc;
        
     /(.*)[\n]+(.*)/;
    if (length($1)>0)
	{my @segm=split(/[\s\,\;\:\'ï¿½ï¿½\(\)\[\]]+/,$1);
    push(@lignec, @segm);}
    $lignecc=$2;
    #print "1:", $1,"\n";
#print "2:", $2,"\n";
    #traiter la phrase(@lignec);
    
     #traiter la phrase(@lignec);
	for ( my $i = 0; $i <= $#lignec; $i++ ) {
		$lignec[$i]=~s/[\*"ï¿½ ï¿½\#\=\$]//g;
		$lignec[$i]=~s/^[\-\(\[\)\]]*//g;
		$lignec[$i]=~s/[0-9]//g;
		#$lignec[$i]=~s/[\"\«\»\‹\›\“\”\„\'\‘\’\‚\…\!\¡\¿\(\)\[\]\{\}\¨\´\`\^\ˆ\~\˜\¸\#\*\,\.\:\;\·\•\¯\—\_\|\¦\†\†\§\¶\©\®\™\&\@\/\\\\µ\<\>\=\=\=\˜\=\±\-\+\×\÷\/\%\‰\¼\½\¾\¹\²\³\º\ª\ƒ\'\'\8\¬ \~\ Ø\*\·\<\>\a\ß\µ]*//g;
		$lignec[$i]=~s/^[\·\—\_\-]//g;
		$lignec[$i]=~s/[\·\—\_\-]$//g;
		$lignec[$i]=~s/[\"\«\»\‹\›\“\”\„\'\‘\’\‚\…\!\¡\¿\(\)\[\]\{\}\¨\´\`\^\ˆ\~\˜\¸\#\*\,\.\:\;\•\¯\|\¦\†\†\§\¶\©\®\™\&\@\/\\\\µ\<\>\=\=\=\˜\=\±\+\×\÷\/\%\‰\¼\½\¾\¹\²\³\º\ª\ƒ\'\'\8\¬ \~\ Ø\*\·\<\>\a\ß\µ]*//g;

if (length($lignec[$i])>0)
	  {my $pivot = lc( $lignec[$i] );
		$freq{$pivot}++;
	  for ( my $j = 0; $j <= $#lignec; $j++ ) {
	      if ( ( $j >= 0 ) and
		   ( $j <= $#lignec ) and 
		   ( $j != $i ) ) {
		$lignec[$j]=~s/[\*"ï¿½ ï¿½]//g;
		$lignec[$j]=~s/^[\-\(\[\)\]]*//g;
		$lignec[$j]=~s/[0-9]//g;
		$lignec[$j]=~s/^[\·\—\_\-]//g;
		$lignec[$j]=~s/[\·\—\_\-]$//g;
		$lignec[$j]=~s/[\"\«\»\‹\›\“\”\„\'\‘\’\‚\…\!\¡\¿\(\)\[\]\{\}\¨\´\`\^\ˆ\~\˜\¸\#\*\,\.\:\;\•\¯\|\¦\†\†\§\¶\©\®\™\&\@\/\\\\µ\<\>\=\=\=\˜\=\±\+\×\÷\/\%\‰\¼\½\¾\¹\²\³\º\ª\ƒ\'\'\8\¬ \~\ Ø\*\·\<\>\a\ß\µ]*//g;


		if ((length($lignec[$j])>0) && ($lignec[$j] ne $lignec[$i]))
{
		  my $cooccurrent = lc ($lignec[$j]);
		  $cooc{$pivot}{$cooccurrent}++;
}
	      } 
}
	  } 
      } 
    
	
    @lignec=();
    }
    if ($lignecc ne $ligne)
    {my @segm=split(/[\s\,\;\:\'ï¿½ï¿½\#\=\$\(\)\[\]]+/,$lignecc);
    push(@lignec, @segm);
    }
    if (!($ligne=~/[\.\!\?]/))
    { #on teste si la ligne est vide
        my $lignet=$ligne;
        $lignet=~s/\s//g;
       # print "ici", $ligne,"\n";
        # print "la", $lignet, length($lignet), $#lignec, "\n",;
        # print "et ici", $ligne,"\n";
        if ((length($lignet)==0)&&($#lignec>0))
        {#traiter la phrase(@lignec);
            
             #traiter la phrase(@lignec);
	for ( my $i = 0; $i <= $#lignec; $i++ ) {
		$lignec[$i]=~s/[\*"ï¿½ ï¿½]//g;
		$lignec[$i]=~s/^[\-\(\[\)\]]*//g;
		$lignec[$i]=~s/[0-9]//g;
		$lignec[$i]=~s/^[\·\—\_\-]//g;
		$lignec[$i]=~s/[\·\—\_\-]$//g;
		$lignec[$i]=~s/[\"\«\»\‹\›\“\”\„\'\‘\’\‚\…\!\¡\¿\(\)\[\]\{\}\¨\´\`\^\ˆ\~\˜\¸\#\*\,\.\:\;\•\¯\|\¦\†\†\§\¶\©\®\™\&\@\/\\\\µ\<\>\=\=\=\˜\=\±\+\×\÷\/\%\‰\¼\½\¾\¹\²\³\º\ª\ƒ\'\'\8\¬ \~\ Ø\*\·\<\>\a\ß\µ]*//g;

if (length($lignec[$i])>0)
	  {my $pivot = lc( $lignec[$i] );
		$freq{$pivot}++;
	  for ( my $j = 0; $j <= $#lignec; $j++ ) {
	      if ( ( $j >= 0 ) and
		   ( $j <= $#lignec ) and 
		   ( $j != $i ) ) {
		$lignec[$j]=~s/[\*"ï¿½ ï¿½\#\$\=]//g;
		$lignec[$j]=~s/^[\-\(\[\)\]]*//g;
		$lignec[$j]=~s/[0-9]//g;
		$lignec[$j]=~s/^[\·\—\_\-]//g;
		$lignec[$j]=~s/[\·\—\_\-]$//g;
		$lignec[$j]=~s/[\"\«\»\‹\›\“\”\„\'\‘\’\‚\…\!\¡\¿\(\)\[\]\{\}\¨\´\`\^\ˆ\~\˜\¸\#\*\,\.\:\;\•\¯\|\¦\†\†\§\¶\©\®\™\&\@\/\\\\µ\<\>\=\=\=\˜\=\±\+\×\÷\/\%\‰\¼\½\¾\¹\²\³\º\ª\ƒ\'\'\8\¬ \~\ Ø\*\·\<\>\a\ß\µ]*//g;

		if ((length($lignec[$j])>0) && ($lignec[$j] ne $lignec[$i]))
{
		  my $cooccurrent = lc ($lignec[$j]);
		  $cooc{$pivot}{$cooccurrent}++;
}
	      } 
}
	  } 
      } 
            
	
         @lignec=();
        }
        else
        {
        #($ligne=~/[ ]*/);
        my @segm=split(/[\s\,\;\:\'ï¿½ï¿½\=\$\#\(\)\[\]]+/,$ligne);
    
        push(@lignec,@segm);
  #print "icisuite", $#lignec," ",$#segm, "\n";
    }
    }
} 

foreach my $pivot ( sort keys %cooc ) {
        if ((length($pivot)>0)&&($pivot ne "-"))
        {print $pivot, "\t", $freq{$pivot},"\t";
  my %cooc_pivot = %{ $cooc{$pivot} };
  foreach my $cooccurrent
 
 ( sort { $cooc_pivot{$b} <=> $cooc_pivot{$a} or $a cmp $b} 
           keys %cooc_pivot ) {
               if ((length($cooccurrent)>0)&&($pivot ne "-"))
               {print  $cooccurrent, "[", $cooc_pivot{$cooccurrent},",\t";
               }
 }
  print "\n";
        }
}


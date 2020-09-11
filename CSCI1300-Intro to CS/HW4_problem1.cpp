# include <iostream>
using namespace std;

void listCodonPositions (string DNA, string Codon) // Problem 1. The function checks locations of the codons within the human, mouse, and unknown DNA.
{
    string group;
    int index;
    for (index = 0; index < DNA.length (); index++)
         {
            group = DNA.substr(index, 3);
            if (group == Codon)
                cout << index + 1 << " ";
         }
}

float calcSimilarity (string Seq1, string Seq2) // Function compares the two strings to find the hamming distance, then calculates the similarity of the strings. Returns that similarity to the main function.
{
    if (Seq1.length () != Seq2.length () )
    {
        return 0;
    }
    int index;
    float similarity_score, hamming = 0;
    for (index = 0; index < Seq1.length (); index++)
    {
        if (Seq1[index] != Seq2[index])
        {
            hamming = hamming + 1;
        }

    }
    similarity_score = ((Seq1.length () - hamming) / Seq1.length ());
    return similarity_score;
}

float compareDNA (string dbSequence, string userSequence) // Function creates a substring at each letter of the large string that is the length of the input string. Then sends those strings to calcSimilarity to find the similarity between the two.
{
    int index, index2;
    float similarity = 0, maxSimilarity = 0, hamming = 0;
    for (index = 0; index < dbSequence.length (); index++)
    {
        string subSeq = dbSequence.substr (index, userSequence.length () );
        similarity = calcSimilarity (userSequence, subSeq);
        if (maxSimilarity < similarity) // Then compares all of those similarities to find the largest similarity for that DNA string.
        {
            maxSimilarity = similarity;
        }
    }
    return maxSimilarity;
}

int main ()
    {
        string humanDNA = "CGCAAATTTGCCGGATTTCCTTTGCTGTTCCTGCATGTAGTTTAAACGAGATTGCCAGCACCGGGTATCATTCACCATTTTTCTTTTCGTTAACTTGCCGTCAGCCTTTTCTTTGACCTCTTCTTTCTGTTCATGTGTATTTGCTGTCTCTTAGCCCAGACTTCCCGTGTCCTTTCCACCGGGCCTTTGAGAGGTCACAGGGTCTTGATGCTGTGGTCTTCATCTGCAGGTGTCTGACTTCCAGCAACTGCTGGCCTGTGCCAGGGTGCAGCTGAGCACTGGAGTGGAGTTTTCCTGTGGAGAGGAGCCATGCCTAGAGTGGGATGGGCCATTGTTCATG";
        string mouseDNA = "CGCAATTTTTACTTAATTCTTTTTCTTTTAATTCATATATTTTTAATATGTTTACTATTAATGGTTATCATTCACCATTTAACTATTTGTTATTTTGACGTCATTTTTTTCTATTTCCTCTTTTTTCAATTCATGTTTATTTTCTGTATTTTTGTTAAGTTTTCACAAGTCTAATATAATTGTCCTTTGAGAGGTTATTTGGTCTATATTTTTTTTTCTTCATCTGTATTTTTATGATTTCATTTAATTGATTTTCATTGACAGGGTTCTGCTGTGTTCTGGATTGTATTTTTCTTGTGGAGAGGAACTATTTCTTGAGTGGGATGTACCTTTGTTCTTG";
        string unknownDNA = "CGCATTTTTGCCGGTTTTCCTTTGCTGTTTATTCATTTATTTTAAACGATATTTATATCATCGGGTTTCATTCACTATTTTTCTTTTCGATAAATTTTTGTCAGCATTTTCTTTTACCTCTTCTTTCTGTTTATGTTAATTTTCTGTTTCTTAACCCAGTCTTCTCGATTCTTATCTACCGGACCTATTATAGGTCACAGGGTCTTGATGCTTTGGTTTTCATCTGCAAGAGTCTGACTTCCTGCTAATGCTGTTCTGTGTCAGGGTGCATCTGAGCACTGATGTGGAGTTTTCTTGTGGATATGAGCCATTCATAGTGTGGGATGTGCCATAGTTCATG";
        string Codon;
        cout << "enter codon:" << endl;
        cin >> Codon;
        while (Codon != "*") // While loops so the loop can be repeated as many times as they want.
        {
            cout << "human: ";
            listCodonPositions (humanDNA, Codon);
            cout << endl;
            cout << "mouse: ";
            listCodonPositions (mouseDNA, Codon);
            cout << endl;
            cout << "unknown: ";
            listCodonPositions (unknownDNA, Codon);
            cout << endl;
            cout << "enter codon:" << endl;
            cin >> Codon;
        }
        string Seq1;
        string Seq2;
        cout << "enter sequence 1:" << endl;
        cin >> Seq1;
        while (Seq1 != "*")
        {
            cout << "enter sequence 2:" << endl;
            cin >> Seq2;
            float Similarity = calcSimilarity (Seq1, Seq2);
            cout << "similarity: " << Similarity << endl;
            cout << "enter sequence 1:" << endl;
            cin >> Seq1;
        }
        string userSeq;
        cout << "enter user sequence:" << endl;
        cin >> userSeq;
        while (userSeq != "*")
        {
            float resultHuman = compareDNA (humanDNA, userSeq);
            float resultMouse = compareDNA (mouseDNA, userSeq);
            float resultUnknown = compareDNA (unknownDNA, userSeq); // Using the largest similarity from each DNA string, compares those to output the DNA with the largest similarity.
            if ((resultHuman >= resultMouse) && (resultHuman >= resultUnknown))
                cout << "Human" << endl;
            if ((resultMouse > resultHuman) && (resultMouse >= resultUnknown))
                cout << "Mouse" << endl;
            if ((resultUnknown > resultHuman) && (resultUnknown > resultMouse))
                cout << "Unknown" << endl;
            cout << "enter user sequence:" << endl;
            cin >> userSeq;
        }
    }
